/* 
	Item.c 
	runcmd.c - Execute a command as a subprocess. 

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

#include "Item.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

ITEM *criar_item(int chave, int pid, int status, int ground, char* nome) {
    ITEM *item = (ITEM *)malloc(sizeof(ITEM));
    item->chave = chave;
    item->pid = pid;
    item->status = status;
    item->ground = ground;
    if (nome != NULL) strcpy(item->nome,nome);
    else nome = NULL;
    return item;
}

void apagar_item(ITEM **item) {
  if (item != NULL) {
    free(*item);
    *item = NULL;
  }
}

void imprimir_item (ITEM *item) {
	printf("%d",item->chave);
}
