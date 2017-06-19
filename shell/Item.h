/* 
  Item.h

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

#ifndef ITEM_H
#define	ITEM_H

typedef struct  {
	char nome[50];
  	int chave;
  	int pid;
  	int status;  /* Define se esta running (1) ou stopped (0) */
  	int ground; /* Define se esta background (1) ou foreground (0) */
} ITEM;

ITEM *criar_item(int chave, int pid, int status, int ground, char* nome);
void apagar_item(ITEM **item);
void imprimir_item(ITEM *item);

#endif	/* ITEM_H */

