/* 
	listaDinamica.c

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

#include <stdio.h>
#include <stdlib.h>
#include "listaDinamica.h"
#include "Item.h"
#include <string.h>

typedef struct NO {
    ITEM *item;
    struct NO *proximo;
} NO;

struct lista {
    NO *inicio;
    NO *fim;
};

LISTA *criarLista() {
    LISTA *lista = (LISTA*)malloc(sizeof(LISTA));
    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

void inserirElemento (LISTA *lista, int chave, int pid, int status, int ground, char *nome){

    NO *novo = (NO*)malloc(sizeof(NO));
    novo->item = criar_item(chave,pid, status, ground,nome);

    if (lista->inicio == NULL) {
        novo->proximo = NULL;
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        NO *paux = lista->inicio, *pant = NULL;
        while (paux != NULL && paux->item->chave < chave) {
            pant = paux;
            paux = paux->proximo;
        }
        if (pant == NULL) {
            novo->proximo = lista->inicio;
            lista->inicio = novo;
        } else {
            pant->proximo = novo;
            novo->proximo = paux;
            if (paux == NULL) lista->fim = novo;
        }
    }
}

void imprimirLista(LISTA *lista) {
    NO *paux;
    char status[20], ground;

    for (paux = lista->inicio; paux != NULL; paux=paux->proximo ){  

        if (paux->item->nome != NULL) {
        
            if(paux->item->status == 0) strcpy(status, "stopped");
            else strcpy(status, "running");

            if (paux->item->ground) ground = '&';
            else ground = ' ';

            if (paux == lista->fim) printf("[%d]%c\t%s\t%s %c\n", paux->item->chave,'+',status,paux->item->nome,ground);        
            else if (paux->proximo == lista->fim) printf("[%d]%c\t%s\t%s %c\n", paux->item->chave,'-',status,paux->item->nome,ground);        
            else printf("[%d]%c\t%s\t%s %c\n", paux->item->chave,' ',status,paux->item->nome,ground);        

        } else {

            if(paux->item->status == 0) strcpy(status, "stopped");
            else strcpy(status, "running");

            if (paux->item->ground) ground = '&';
            else ground = ' ';

            if (paux == lista->fim) printf("[%d]%c\t%s\n", paux->item->chave,'+',status);        
            else if (paux->proximo == lista->fim) printf("[%d]%c\t%s\n", paux->item->chave,'-',status);        
            else printf("[%d]%c\t%s\n", paux->item->chave,' ',status);        

        }
    }
}

void buscarChave(LISTA *lista, int chave) {
    NO *paux;
    for (paux = lista->inicio; paux->item->chave != chave; paux=paux->proximo );
    imprimir_item(paux->item);
}

void buscarPosicao(LISTA *lista, int posicao) {
    
    int i = 0;
    NO *paux = lista->inicio;
    while (i < posicao) {
        paux = paux->proximo;
        i++;
    }
    imprimir_item(paux->item);
}

void removerChave(LISTA *lista, int chave) {
    NO *patual = lista->inicio, *prem = NULL;
    while (patual != NULL && patual->item->chave!=chave) {
        prem = patual;
        patual = patual->proximo;
    }
    if (prem != NULL) {
        prem->proximo = patual->proximo;
        if (patual->proximo == NULL) lista->fim = prem;
    } else {
        lista->inicio = patual->proximo;
    }
    apagar_item(&patual->item);
}

void removerPosicao(LISTA *lista, int posicao) {
    int i = 0;
    NO *patual = lista->inicio, *prem = NULL;
    while (patual!=NULL && i != posicao) {
        prem = patual;
        patual = patual->proximo;
        i++;
    }
    if (prem != NULL) {
        prem->proximo = patual->proximo;
        if (patual->proximo == NULL) lista->fim = prem;
    } else {
        lista->inicio = patual->proximo;
    }
    apagar_item(&patual->item);
}

void inverterLista(LISTA *lista) {
    NO *patual = lista->inicio, *pnext = lista->inicio->proximo, *pant = NULL;
    lista->fim = patual;
    while (patual != NULL) {
        patual->proximo = pant;
        pant = patual;
        patual = pnext;
        if (pnext != NULL) pnext = pnext->proximo;
    }
    lista->inicio = pant;
}

char  estaOrdenada(LISTA *lista) {
    NO *atual = lista->inicio;
    int crescente = 1, decrescente = 1;
    
    while (atual->proximo != NULL && crescente && decrescente) {
        if (atual->item->chave > atual->proximo->item->chave) crescente = 0;
        if (atual->item->chave < atual->proximo->item->chave) decrescente = 0;
    }
    
    if (crescente) return 'C';
    else if (decrescente) return 'D';
    else return 'B';
    
}

void deletarLista(LISTA **lista) {
    NO *paux = (*lista)->inicio, *prem;
    while (paux != NULL) {
        prem = paux;
        paux = paux->proximo;
        apagar_item(&prem->item);
    }
    free(*lista);
}

int tamanhoLista (LISTA *lista) {
    int tamanho = 0;
    NO *paux;
    for (paux = lista->inicio; paux != NULL; paux=paux->proximo ) tamanho++;
    return tamanho;
}

int getStatus(LISTA *lista, int chave) { 
    NO *paux; 
    for (paux = lista->inicio; paux != NULL; paux=paux->proximo ){ 
        if (paux->item->chave == chave) return paux->item->status; 
    } 
    return -1; 
}

int getPIDbyChave (LISTA *lista, int chave) { 
    NO *paux; 
    
    for (paux = lista->inicio; paux != NULL; paux=paux->proximo ){ 
        if (paux->item->chave == chave) return paux->item->pid; 
    } 
    return -1; 
}

int getPIDbyPosition (LISTA *lista, int position) { 
    NO *paux;
    int i;

    if (position > tamanhoLista(lista)) return -1;

    for (i = 0, paux = lista->inicio; i < position; i++, paux=paux->proximo);
    
    return paux->item->pid;
}

void setStatus (LISTA *lista, int chave, int status) {
    NO *paux; 
    for (paux = lista->inicio; paux != NULL; paux=paux->proximo ){ 
        if (paux->item->chave == chave) {
            paux->item->status = status;
            return;
        }
    } 
    printf("Não encontrou esta chave\n");
    return;
}

int proximaChave (LISTA *lista) {

    if (tamanhoLista(lista) == 0) return 1;
    else return lista->fim->item->chave + 1;

}
