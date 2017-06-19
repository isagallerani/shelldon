int fork(void); "garfo"
	   # 
       |
       |
      / \
     #   #



while() {
	prompt()
	read_line(buffer);
	if(exist(buffer))
		execute(buffer); // localiza, carrega na ram, executa
}

1. parser
2. fork
3. exec
4. wait

---- / Aula 26 - 04 / ----

---- / Aula 03 - 05 / ----

Prazo de entrega: 2 semanas, contando a partir do dia 3 de maio

exercicio:
organizar projeto so2
ler documentação disponível no gitlab
fazer exercicio runcmd
usar tag nos pushes
fazer a lib	
fazer o makefile

Lib:

#include <stdlib.h>

void foo(void)
{

} 

compilar: gcc main.c foo.c -o main

compilar lib tudo junto: gcc main.c minhalib.o
 

gcc main.c -lm

ar -> concatena arquivos

ar rcs libfoobar.a foo.o bar.o -> concatena os arquivos na libfoobar

gcc main.c  -L. -lfoobar  -o main -> linka com a lib

--- shared library

- -fPIC -> codigo relocavel, linux usa por padrão

gcc -fPIC -c foo.c
gcc -fPIC -c bar.c

gcc --shared foo.o bar.o -o libfoobar.so //--shared ->estou fazendo uma bibiblioteca dinamica

// onde o kernel vai buscar a library pra carregar na ram - seta na hora para o terminal
export LD_LIBRARY_PATH=.

// link
gcc main.c -o main -L. -lfoobar

//execução
./main

// outro jeito
substitui export LD_LIBRARY_PATH=.

readlf mostra informações sobre o arquivo

na hora da linkagem use o cmg rpath

gcc main.c -L. -WL,-rpath="." -lfoobar -o main

ou
// procure no lugar onde vc encontrou o executavel
gcc main.c -L. -WL,-rpath="\$ORIGIN" -lfoobar -o main -> \para usar $ como literal

gcc main.c -L. -WL,-rpath="\$ORIGIN/../bin" -lfoobar -o main -> volta um na raiz e vai para o bin

---- / Aula 10 - 05 / ----

//redirect.c

#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp; // FILE é uma struct
	fp = fopen("out.txt", w);
	fprintf(fp, "Hello World\n");

	printf("Hello Standard Output\n"); // chama a fprintf -> chama a fwrite
	fprintf(stdout, "Hello Standard Output\n"); // arquivo stdout já vem aberto, standard output


	fclose(fp);
	return EXIT_SUCCESS;
}

//io.c

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include < unistd.h>

#define sysfatal do{if (exp){fprintf(stderr, "%s : %d : %s", __FILE__, __LINE__, strerror(errno));exit(1)\
}} while (0)

int main()
{
	int fd;
	fd = open("out.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	sysfatal(rs<0);
	printf("fd is %d\n", fd);
	rs = write(1, "oh\n", 3); // i/0 <-> 0 ou 1
	sysfatal(rs < 0);
	close(fd);
	return EXIT_SUCCESS;
}

// para usar a biblioteca dele
./configure --prefix=home/aluno/diretorio # configura o diretorio pra instalacao

//depois
make
make install

cd local/lib

gcc -I./home/aluno/local/include -L/home/aluno/diretorio redir.c -o redir  
								 -L$HOME/local/lib


---- / Aula 17 - 05 / ----

gcc main.c circle.c rectangle.c square.c -o main 
	- pre-processamento, codigo fonte ->cod. fonte com diretivas resolvidas - cpp

	- compilação, produz um objeto, circle.o, rectangle.o, ..., código de máquina assembly - c

	- linkagem (ld), linker dinamico vai recauclular os endereços - ld
		- linkagem dos .os
		- 
	gcc -c -I. main.c -> para na etapa de geraçao de codigo de maquina, objeto
	gcc main.o circle.o rectangle.o square.o -o main, só faz linkagem

alvo: pre-requisitos
	receita, como fazer

# Make

make -f makefile-00 // -f para passar um nome de arquivo para o make
data alvo mais recente q a data dos objetos -> não precisa recompilar

touch muda a data do aqruivo, caso ele nao existir
$^ -> pre-requisitos
$@ -> alvo
$< -> primeiro pre-requisito

%.o : %.c -> qq coisa .o depende de qq coisa .c

flag -M -> mostra os includes de um arquivo
flag -MM -> mostra as bibliotecas inclusas num arquivo, apenas do projeto

# detectando headers automaticos

makefiles = %(OBJECTS:%.o=%.d) troca os .o por .d
include $(makefiles) # inclue os arqs main.d
%.d : %.c
	-MM : criar uma regra de makefile
	gcc -I. -MM -MT '$(<:.c=.o) $@' $< >$@ # redireciona para o alvo
	-MT -> o arg na frente diz oq fica na regra alvo
	'$(<:.c=.o) $@' : ler os pre-requisitos trocando .c por .o 

---- / Aula 24 - 05 / ----

fecha o standard output, close no descritor de arqs1, abre um arquivo qq
este arq vai ser associado ao um, dou exec, printf escreve no arquivo passado

// Pipeline
echo "I have" $(ls -l | grep "\.pdf" | wc -l)" pdf files here"

rodar os 3 programas
	- 3 forks
	- 3 processos filhos + o pai
- redirecionar a saida de cada um para redirecionar para outro

--Pipe no kernel: dispositivo que implementa o pipeline
  				- é um buffer, um array do tipo fifo
  				- cria descritor de arquivo para o buffer
  				- cria 2 buffer, um para escrita outro pra leitura

- escreve ->| buffer | -> le 
- escreve e le de arquivo
- funcao pipe: cria no kernel um buffer e retorna um arquivo

//pipe.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/tmp/bar/include/posixeg/debug.h"


int pipe(int pipefd[2]);

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>

int pipe2(int pipefd[2], int flags);


int main(int argc, char const *argv[])
{
	int rs;
	
	rs = pipe(pipefd); //pos 0 -> ler do pipe , pos 1 - escrever no pipe // I/O

	sysfatal(rs<0);
	write(pipefd[1], "Hello", 5);
	read(pipefd[0], buffer, 5);
	buffer[5] = '\0';
	printf("%s\n", buffer);

	return EXIT_SUCCESS;
}

// versao 2

int main(int argc, char const *argv[])
{
	int rs, pipefd[2], pid, status;
	char buffer[512];
	
	rs = pipe(pipefd); //pos 0 -> ler do pipe , pos 1 - escrever no pipe // I/O

	pid = fork();
	sysfatal(rs<0);
	if (pid > 0) // pai
	{
		wait(&status);
		rs = read(pipefd[0], buffer, 11); //pipefd[0] : lendo do descritor de arquivo 3, (1, 2, reservado para stdoutput stderror)
		buffer[rs] = '\0';
		printf("filho: %s\n", buffer );
	}
	else //filho
	{
		write(pipefd[1], "hello daddy", 11); //pipefd[1] : escrevendo do descritor de arquivo 4
	}

	write(pipefd[1], "Hello", 5);
	read(pipefd[0], buffer, 5);
	buffer[5] = '\0';
	printf("%s\n", buffer);

	return EXIT_SUCCESS;
}

// versao 3
	  __________________
	0|pts/5  |1 (utilização)
	1|pts    |1 fechar ->
	2|pts    |1
	3|pipein |1
	4|pipeout|1

	  __________________
	0|pts/5  |1 (utilização)
	1|pts    |0
	2|pts    |1
	3|pipein |1
	4|pipeout|1 (mudar saida padrao para o 4)

funcao dup: duplica um descritor de arquivo
	  __________________
	0|pts/5  |1 (utilização)
	1|pts    |0 dup -> 1 aponta para o 4
	2|pts    |1
	3|pipein |1
	4|pipeout|2 (mudar saida padrao para o 4) - pipefd[1] ; 2 descritores apontando para o pipeout

do outro lado...
	  __________________
	0|pts/5  |1 dup -> 0 aponta para o 3 ; dup(pipefd[0])
	1|pts    |0  
	2|pts    |1
	3|pipein |2 
	4|pipeout|2 

p1 -> buffer -> p2 

implementacao...

p1 | p2 | p3

jeito recursivo
shel
    |
    p3
    |
    p2
    |
    p1 
outro jeito usado n bash ...
3 forks
2 pipes
(p1 p2 p3 p4) -> job
- rodar em paralelo

p1 -> pipe -> p2 -> pipe1 -> p3 -> pipe -> p4
|
produtor - consumidor
a hora q o pipe encher -> produtor é bloqueiado
- inverso é valido
- esperar o último terminar

int main(int argc, char const *argv[])
{
	int rs, pipefd[2], pid, status;
	char *args1[]= {"ls", "-l", NULL};
	char *args2[]= {"wc", "-l", NULL};
	
	rs = pipe(pipefd); //pos 0 -> ler do pipe , pos 1 - escrever no pipe // I/O

	pid = fork();
	sysfatal(rs<0);
	if (pid > 0) // pai
	{
		close(1); // fecha a saida padrao
		dup(pipefd[1]); // duplica  a ponta de escrita do pipe
		close(pipefd[1]); // fecha o descritor 4 
		close(pipefd[0]); // fecha o descritor 4 
		rs = execvp(args1[0], args1); 
		sysfatal(rs<0);
	}
	else //filho
	{
		close(0);
		dup(pipefd[0]); // pega o menor descritor de arqs disponivel e aponta para o pipefd[0], junto c/ o descritor 3
		close(pipefd[0]);
		close(pipefd[1]); // nao vou usar a ponta de escrita, fecho
		execvp(args2[0], args2);
		sysfatal(rs<0);
	}

	write(pipefd[1], "Hello", 5);
	read(pipefd[0], buffer, 5);
	buffer[5] = '\0';
	printf("%s\n", buffer);

	return EXIT_SUCCESS;
}


// runcmd

runcmd(command, &status, io[31]); //retorna o pid se executou, -1 se falhou

fork();
-- exec
- pensar num jeito
if(pid>0)
{
	wait(&status); //valor q o filho terminou
}
else
{
	exec
	escrever no pipe o valor de retorno
	systemcall para ativar o_cloexec
	tenta dar um read(pipe[0]), se a read retornar -1(n conseguiu ler), o pipe foi fechado na exec, ela rodou 
}
exit; -> do pai


cria um branch sobe uma issue no help

---- / Aula 31 - 05 / ----

kill -9 31859

fg -> comando forground

sinal -> interrupção de software para um processo
implementacao de um sinal -> systemcall

sinal 1, 2, 3, 4 ->cada  sinal tem uma ação
 - sinal para matar o processo
 - suspender processo
 - sinal para qdo acessar area de memoria indevida
 - a wait fica bloqueada até que recebe um sinal de termino do processo filho
 - signal interruption -> 


// gerar um sinal -> mecanismo de comunicação assincrono

funcao kill // matar, interromper, continuar um processo parado

// programa kill

#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int pid;
	pid = atoi(argv[1]);
	kill(pid, SIGINT);
	return 0;
}

// versao 2

int main(int argc, char const *argv[])
{
	int pid;
	pid = atoi(argv[1]);
	kill(pid, sinalParaSuspenderProcesso); // SIGCOUNT -> acorda um processo 
	return 0;
}

// kernel implementa uma fila de sinal, aos poucos vai processando
// qualquer sinal desbloqueia a wait
// qualquer sinal desbloqueia uma chamda bloqueante
// sinal SEGSEGV -> mata o processo por segmentation fault

S+ -> + diz quem tem o controle do terminal pts/29

registrar um manipulador para o sinal -> kernel vai mandar sinal de acordo com o manipulador
+ ignorar um sinal
+ suspender um sinal
+ sobrescrever a ação do sinal

// sobrescrever a ação do sinal

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void foo(int signum)
{
	printf("ops");
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, foo);
	signal(SIGTERM, foo);
	while(1)
	{
		sleep(1);
		printf(".");
		fflush(stdout);
	}
	kill(pid, sinalParaSuspenderProcesso); // SIGCOUNT -> acorda um processo 
	return EXIT_SUCCESS;
}

kill -n SIGINT 6743 // manipulou o sinal, nao mata mais o processo
kill -n SIGTERM 6743


SIG_IGN -> sig ignore, igonorar sinal

SIG_DFL -> restauro a ação padrão

sigaction
 - consigo saber quem mandou o sinal

// versao melhor:


sigaction(SIGINT, &act, &act_old); // passa um ponteiro para uma estrutura
- pega todas as informações do sinal, e preencha na estrutura &act_old, e seta no kernel com &act
- &act_old é pra guardar o que ela tinha anteriormente, para voltar como era

assim: 

void main() 
{
	rs = sigaction(SIGINT, NULL, &act);
	sysfatal(rs<0);

	act.sa_handler = foo;
	rs = sigaction(SIGINT, &act, NULL);
	sysfatal(rs<0);
	while(1)
	{
		sleep(1);
		printf(".");
		fflush(stdout);
	}
	(pid, sinalParaSuspenderProcesso); // SIGCOUNT -> acorda um processo 
	return EXIT_SUCCESS;
}

---- / Aula 07 - 06 / ----















