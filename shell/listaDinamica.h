/* 	
	listaDinamica.h
	
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

#ifndef LISTA_H
#define	LISTA_H

typedef struct lista LISTA;

LISTA *criarLista();
void inserirElemento(LISTA *lista, int chave, int pid, int status, int ground, char* nome);
void imprimirLista(LISTA *lista);
void imprimirLista2(LISTA *lista);
void buscarChave(LISTA *lista, int chave);
void buscarPosicao(LISTA *lista, int posicao);
void removerChave(LISTA *lista, int chave);
void removerPosicao(LISTA *lista, int posicao);
void inverterLista(LISTA *lista);
char  estaOrdenada(LISTA *lista);
void deletarLista(LISTA **lista);
int tamanhoLista(LISTA *lista);
int getStatus(LISTA *lista, int chave);
int getPIDbyChave (LISTA *lista, int chave);
int getPIDbyPosition (LISTA *lista, int position);
void setStatus (LISTA *lista, int chave, int status);
int proximaChave (LISTA *lista);

#endif	/* LISTA_H */

