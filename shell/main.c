/* 
	main.c
	
	Shelldon, Copyright(c) 2017, Alessandra Camargo, Alexandre Lara, Lucas Pessoa, Tiago Avellar 
	<552038@comp.ufscar.br, 587117@comp.ufscar.br, 551864@comp.ufscar.br, 551910@comp.ufscar.br>

	Shelldon is derived from POSIXeg, Copyright (c) 2017, Francisco José Monaco <moanco@icmc.usp.br>

	This file is part of Shelldon.

	Shelldon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Shelldon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Our Shelldon repository can be found at https://github.com/SO-II-2017/shelldon
*/

#include <stdlib.h>
#include <stdio.h>
#include "foosh.h"
#include "debug.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include "listaDinamica.h"

#define PROMPT "$> "

int go_on = 1;			/* This variable controls the main loop. */
LISTA *jobs;

void setupSignals () {	
	signal (SIGINT,  SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGCHLD, SIG_IGN);
} 

void redirect (pipeline_t *pipeline) {

	int fd_in, fd_out;

	if ( REDIRECT_STDOUT(pipeline) ) { /* Redirecionamento de saida */
		close(1); /* Fecha o arquivo padrão de saida, para substitui-lo pelo inserido pelo usuario */
		fd_out = open(pipeline->file_out, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
		fatal(fd_out < 0, "open"); 
		dup(fd_out); /* Como o arquivo padrão de saida é -1, então ele preenche com o fd_out */
	}
		
	if ( REDIRECT_STDIN(pipeline) ) { /* Redirecionamento de entrada */
		close(0); /* Fecha o arquivo padrão de entrada */
		fd_in = open(pipeline->file_in,O_RDONLY, S_IRUSR | S_IWUSR);
		fatal(fd_in < 0, "open");
		dup(fd_in); /* Como o arquivo padrão de entrada é -1, então ele preenche com o fd_in */
	}

	return;
}
void authors () {
	printf("\n");
	printf("\tShelldon was created in early 2017 as a practical work of the discipline \n");
	printf("\tOperating Systems 2, offered by Ph.D. F. Monaco at the Instituto de Mathematical\n");
	printf("\tand Computer Sciences (ICMC) of the University of São Paulo (USP).\n");
	printf("\tHere is a list of people who, in some way, make this project become real:\n");
	printf("\tAlessandra de Souza Camargo   <552038@comp.ufscar.br> \n");
	printf("\tAlexandre Da Silva Lara Pinto <587117@comp.ufscar.br> \n");
	printf("\tLucas Eduardo Pessoa          <551864@comp.ufscar.br> \n");
	printf("\tTiago Avellar Fernandes       <551910@comp.ufscar.br> \n");
	printf("\tFrancisco José Monaco         <monaco@icmc.usp.br> \n");
	printf("\n");
}

void ls () {
    char diretorioAtual[100];
    DIR *dir = NULL;
    struct dirent *drnt = NULL; 
 
    getcwd(diretorioAtual,100); /* Recupera o diretorio atual */
 
    dir = opendir(diretorioAtual); /* Abre o diretorio atual */
    if (dir) {

        while ((drnt = readdir(dir))) 
        	if (drnt->d_name[0] != '.')
        		printf("%-30s\n", drnt->d_name); /* Lista todos os arquivos */

        closedir(dir);
    	exit(EXIT_SUCCESS);

    } else printf("Não foi possível abrir essa pasta '%s'\n", diretorioAtual);
}

void pwd () {

	char diretorioAtual[100];
	getcwd(diretorioAtual,100);
	printf("%s\n",diretorioAtual);
	exit(EXIT_SUCCESS);

}

void cd (pipeline_t *pipeline) {

	char diretorioAtual[100];
    DIR *dir = NULL;
    struct dirent *drnt = NULL; 

	if (pipeline->command[0][1] == NULL) chdir(getenv("HOME"));
	else if (!strcmp(pipeline->command[0][1],"..")) chdir("..");
	else {

		getcwd(diretorioAtual,100); /* Recupera o diretorio atual */
    	dir = opendir(diretorioAtual); /* Abre o diretorio atual */

    	if (dir) {

        	while ((drnt = readdir(dir))) {
        		if (!strcmp(drnt->d_name,pipeline->command[0][1])) {
        			chdir(drnt->d_name);
        		}
        	}

        	closedir(dir);
    	} else printf("Não foi possível abrir essa pasta '%s'\n", diretorioAtual);

	}
}

void n_mkdir (pipeline_t *pipeline) {
	int result = mkdir(pipeline->command[0][1], S_IRWXU | S_IRWXG | S_IRWXO); /* Cria o diretorio */
	if (result != 0) printf("Erro, diretorio nao criado!\n");
}

void fjobs () {
	imprimirLista(jobs);
	exit(EXIT_SUCCESS);
}

void bg (char *command) { /* bg — Resume a suspended program without bringing it to the foreground. */
	int bgPid;

	if (getStatus(jobs,atoi(command)) == 1) printf("Processo ja esta em background\n");
	else {

		signal (SIGINT,  SIG_IGN);
		signal (SIGTSTP, SIG_IGN);

		/* Se o processo ja estiver em stopped, ele deve voltar a funcionar */
		bgPid = getPIDbyChave(jobs,atoi(command));
		setStatus(jobs,atoi(command),1);
		kill(bgPid, SIGCONT);
	}
	return;
}

void fg (char *command) { /* continues a stopped job by running it in the foreground */
	int bgPid, aux, status;

	bgPid = getPIDbyChave(jobs,atoi(command));
	removerChave(jobs,atoi(command)); /* Remove o processo da lista de jobs */
	kill(bgPid, SIGCONT); /* Faz com que o processo volte a executar */

	aux = waitpid(bgPid, &status, WUNTRACED); /* Assim que o processo termina de executar, sua execução é testada */
	if (WIFSTOPPED(status)) { /* Parou por um ctrl Z */
		inserirElemento(jobs,proximaChave(jobs),aux,0,0,NULL); 
	} 	

	return;
}

void n_exit () {

	int i;
	for (i = 0; i < tamanhoLista(jobs); i++) {
		kill(getPIDbyPosition(jobs,i),SIGTERM); /* Mata os filhos para finalizar */
	}

	exit(EXIT_SUCCESS);
}

void help () {
  printf("==== Shelldon Help Panel ====\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  printf("%s\n", "mkdir");
  printf("%s\n", "jobs");
  printf("%s\n", "echo");
  printf("%s\n", "exit");
  printf("%s\n", "cd");
  printf("%s\n", "bg");
  printf("%s\n", "fg");
  printf("Use the man command for information on other programs.\n");
}


void clearDone () {

	int i, aux, status;
	for (i=0; i < tamanhoLista(jobs); i++) {
		aux = waitpid(getPIDbyPosition(jobs,i),&status,WNOHANG);
		if (aux > 0) printf("%d done!\n",getPIDbyPosition(jobs,i));
	}
}

void rodaPipe (pipeline_t *pipeline, int *entrada, int *saida, int position) {

	int pid, fdIn, fdOut, aux;

	pid = fork();
	fatal(pid < 0, NULL);

	if (pid > 0) { /* Pai */

		if (entrada != NULL) close(entrada[1]); /* Fecha a escrita, a leitura primeiro */

	} else { /* Filho */

		signal(SIGINT, SIG_DFL); /* Habilita o Ctrl C para o filho */
		signal(SIGTSTP, SIG_DFL); /* Habilita o Ctrl Z para o filho */

		/* Para a saida, há dois tipos, redirecionamento OU escrita em algum arquivo */
		if (position == pipeline->ncommands - 1 && REDIRECT_STDOUT(pipeline)) {
			close(1);
			fdOut = open(pipeline->file_out, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
			fatal(fdOut < 0, "open");
		} else if(saida != NULL) {	/* Caso seja necessario escrever em algum lugar diferente de stdout */
			close(1);
			dup(saida[1]);
			close(saida[0]);
		}

		/* Como na saida, para a entrada da dois tipos: entrada por redirecionamento, ou leitura por algum arquivo */
		if (position == 0 && REDIRECT_STDIN(pipeline)) { /* O primeiro comando é um redirecionamento */
			close(0);
			fdIn = open(pipeline->file_in, O_RDONLY, S_IRUSR | S_IWUSR);
			fatal(fdIn < 0, "open");	
		} else if (entrada != NULL) { /* A leitura vem de algum arquivo */
			close(0); /* Fecha a entrada padrão */
			dup(entrada[0]); /* Transforma a entrada[0] na entrada padrão */
			close(entrada[1]); /* Fecha a escrita na entrada */
		}

		aux = execvp(pipeline->command[position][0],pipeline->command[position]);
		fatal(aux < 0,NULL);
	}

}

void functionPipeline (pipeline_t *pipeline) {

	int i, **pipes, aux;

	/* Cria a matriz que vai armazenar os pipes */
	pipes = (int**)malloc(sizeof(int*) * pipeline->ncommands - 1);
	for (i = 0 ; i < pipeline->ncommands - 1; i++) pipes[i] = (int*)malloc(sizeof(int) * 2);

	/* Cria os vetores de comunicação pipe */
	for (i = 0; i < pipeline->ncommands - 1; i++) {
		aux = pipe(pipes[i]);
		fatal (aux < 0,NULL);
	}

	for (i = 0 ; i < pipeline->ncommands; i++) {

		/* As diferenças nos pipes acontecem quando nós executamos o primeiro ou o ultimo comando, por isso,
		é importante que tenha um if para cada um */

		if (i == 0) { /* Primeiro comando do pipe */
			rodaPipe(pipeline, NULL, pipes[i],0);	
		} else if ( i == pipeline->ncommands - 1) { /* Ultimo comando do pipe */
			rodaPipe(pipeline, pipes[i - 1], NULL, i);
		} else {
			rodaPipe(pipeline, pipes[i - 1], pipes[i],i);			
		}

	}

	return;
}

void runcommand (pipeline_t *pipeline) {

	int aux, status, pid, flagBackGround = 0;

	clearDone(); /* Verifica se algum dos jobs foi finalizado, para que se sim, sair da lista */

	if (pipeline->ncommands > 1) { /* Se o comando inserido foi um pipeline */
		functionPipeline(pipeline);
		return;
	}

	/* Para executar exit, cd ou mkdir não é necessário forkar o processo, por isso, esses comandos são
	   executados antes */
	if (!strcmp(pipeline->command[0][0],"exit")) n_exit(); /* Exit */
	else if (!strcmp(pipeline->command[0][0],"help")) help(); /* Help */
	else if (!strcmp(pipeline->command[0][0],"cd")) { /* CD */
		cd(pipeline);
		return;
	}
	else if (!strcmp(pipeline->command[0][0],"mkdir")) { /* Mkdir */
		n_mkdir(pipeline);
		return;
	} else if (!strcmp(pipeline->command[0][0],"bg")) { /* BG */
		bg(pipeline->command[0][1]);
		return;
	} else if (!strcmp(pipeline->command[0][0],"fg")) { /* FG */
		fg(pipeline->command[0][1]);
		return;
	}
	else if (!strcmp(pipeline->command[0][0],"authors")) authors(); /* Help */

	/* Verifica se é background */
	if (pipeline->ground == BACKGROUND) flagBackGround = 1;

	pid = fork(); /* Forka, criando um processo filho para execução */

	if (pid > 0) { /* Esta no pai */

		/* Caso seja uma instrução de redirecionamento de entrada ou saida, ou sejja uma instrução que foi chamada
		   em background (&) o pai não deve esperar sua finalização, portanto, o comando wait não deve ser executado.
		   O if abaixo garante isto */
		if ( (!REDIRECT_STDOUT(pipeline) && !REDIRECT_STDOUT(pipeline)) && !flagBackGround) { 

			aux = waitpid(pid, &status, WUNTRACED);
			if (WIFSTOPPED(status)) { /* Parou por um ctrl Z */
				inserirElemento(jobs,proximaChave(jobs),aux,0,0,NULL); 
			} 
		} 

	} else { /* Filho */

		redirect(pipeline); /* Verifica se necessita de um redirecionamento */
	
		if (!strcmp(pipeline->command[0][0],"ls") && (pipeline->command[0][1] == NULL)) ls(); /* LS */
		else if (!strcmp(pipeline->command[0][0],"pwd")) pwd(); /* Pwd */
		else if (!strcmp(pipeline->command[0][0],"jobs")) fjobs(); /* Jobs */
		else {
			if (!flagBackGround) { /* Se não for um comando de execução background, os sinais devem ser ativos para
				que os filhos possam ser mortos */
				signal(SIGINT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);
			}
			if(!strcmp(pipeline->command[0][0],"echo") && (!strcmp(pipeline->command[0][1], "$pwd"))) pwd();
			aux = execvp(pipeline->command[0][0],pipeline->command[0]); /* Executa o comando, se não for um dos anteriores */
			if (aux < 0) exit(EXIT_FAILURE);
		} 

	}

	if (flagBackGround) { /* Se o comando for background, deve ser inserido na lista */
		printf("[%d]\t%d\n",proximaChave(jobs),pid);
		inserirElemento(jobs,proximaChave(jobs),pid,1,1,pipeline->command[0][0]);
	}

	return;
}

int main (int argc, char **argv) {

	buffer_t *command_line;

	pipeline_t *pipeline;

	command_line = new_command_line ();
	pipeline = new_pipeline ();

	jobs = criarLista();

	while (go_on) {

		setupSignals();  /* Seta os sinais de comando */

		printf ("%s ", PROMPT);
		fflush (stdout);

		read_command_line (command_line);

		/* Parse command line. */
		if (!parse_command_line (command_line, pipeline) || 1)
			if ( pipeline->ncommands > 0 )  
				runcommand(pipeline);

	}

	release_command_line (command_line);
	release_pipeline (pipeline);

	return EXIT_SUCCESS;
}
