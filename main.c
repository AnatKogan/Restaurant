#define _CRT_SECURE_NO_WARNINGS
#include"resturant.h"

int main()
{
	ListMana PManot;
	//Defining menager list for the list of dishes
	Table* tables[MAXTABLES];
	//Defining array of pointers to struct of tables
	FILE* Pin;
	if ((Pin = fopen("Instructions.txt", "r")) == NULL)
		//Opening the instructions file and checking if the opening has succeeded
		Error_Msg("Opening Instructions file didnt work");
	int i, command = 0, quantity = 0, TableNumber = 0;
	char ManaName[MAX];
	PManot.head = PManot.Tail = NULL;
	PManot.amount = 0;
	ResetTables(tables);
	//Calling the function that reset the tables
	while ((fscanf(Pin, "%d ", &command) != EOF))
		//Scanning the command in the file
	{
		switch (command)
		{
		case 1:
			CreateProducts(&PManot);
			//Creating the menu
			break;
		case 2:
			fscanf(Pin, "%s %d", ManaName, &quantity);
			//Scanning the rest of the commands in the correct order
			AddItems(ManaName, quantity, &PManot);
			//Adding quantity to the dish in the menu
			break;
		case 3:
			fscanf(Pin, "%d %s %d", &TableNumber, ManaName, &quantity);
			//Scanning the rest of the commands in the correct order
			OrderItem(TableNumber, ManaName, quantity, &PManot, tables);
			//Adding order to a table
			break;
		case 4:
			fscanf(Pin, "%d", &TableNumber);
			RemoveItem(TableNumber, tables);
			//Removing the last order from a table
			break;
		case 5:
			fscanf(Pin, "%d", &TableNumber);
			RemoveTable(TableNumber, tables);
			//Closing the table
			break;
		default:
			printf("\nWrong command input in txt file.");
			break;
		}

	}
	FreeManotList(&PManot);
	for (i = 0; i < MAXTABLES; i++)
	{
		FreeManotList(&tables[i]->Tablefood);
		free(tables[i]);
	}
	//Freeing the memory
	fclose(Pin);
	//Closing instructions file
}
