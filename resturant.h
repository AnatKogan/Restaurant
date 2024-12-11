#ifndef _RESTURANT_H

#define _CRT_SECURE_NO_WARNINGS
#define _RESTURANT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 50
#define MAXTABLES 50

typedef struct Mana
{
	char* Name;
	int Price;
	int Quantity;
	struct Mana* next;
}Mana;

typedef struct ListMana
{
	Mana* head;
	Mana* Tail;
	int amount;
}ListMana;

typedef struct Table
{
	int IsReturned;
	ListMana Tablefood;
}Table;

//Defining buildings

void CreateProducts(ListMana*);
void Error_Msg(char*);
void FreeManotList(ListMana*);
void AddItems(char*, int, ListMana*);
void OrderItem(int, char*, int, ListMana*, Table* tables[]);
void RemoveItem(int, Table* tables[]);
void RemoveTable(int, Table* tables[]);
void ResetTables(Table* tables[]);
//Declaring functions
#endif
