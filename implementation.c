#include"resturant.h"

void CreateProducts(ListMana* L)
//Function that creates the list of dishes from file
{
	int i, IsSameName, temp_quantity, temp_price;
	char temp_name[MAX];
	Mana* temp;
	FILE* p;
	if ((p = fopen("Manot.txt", "r")) == NULL)
		Error_Msg("\n Opening file didnt work");
	//Checking if the file opening succeeded
	while ((fscanf(p, "%s %d %d", temp_name, &temp_quantity, &temp_price)) != EOF)
	{//A loop that works while there is something in the file
		if (temp_price < 0 || temp_quantity < 0)
			//Checking the price and quantity aren't negative numbers 
		{
			printf("\nThere was problem with %s price/quantity in the file and was not added.", temp_name);
			continue;
		}
		temp = L->head;
		//Setting temp to look at the first dish
		IsSameName = 0;
		while (temp != NULL)
		{
			if (strcmp(temp->Name, temp_name) == 0)
				//Checking if there is already a dish with this name on the list
			{
				printf("\n%s is already in the list and was not added again.", temp_name);
				IsSameName = 1;
				break;
			}
			temp = temp->next;
			//Advencing temp
		}
		if (IsSameName)
			//If there is already a dish with that name continues to the next item in the file without adding current one
		{
			temp = temp->next;
			continue;
		}
		if ((temp = (Mana*)malloc(sizeof(Mana))) == NULL)
		{ //Allocating memory for temp and checking if it succeeded
			FreeManotList(L);
			Error_Msg("\nThere was a problem allocation the memory");
		}
		if ((temp->Name = (char*)malloc((strlen(temp_name) + 1) * sizeof(char))) == NULL)
		{ //Allocating memory for the name and checking if it succeeded
			FreeManotList(L);
			Error_Msg("\nThere was a problem allocation the memory for the name");
		}
		strcpy(temp->Name, temp_name);
		temp->Quantity = temp_quantity;
		temp->Price = temp_price;
		//Putting relevent data into temp
		temp->next = NULL;
		if (L->head == NULL)
			L->head = temp;
		else
			L->Tail->next = temp;
		L->Tail = temp;
		//Adding to the list from the tail
		L->amount++;
		//Updating the amount
	}
	printf("The kitchen was created");
	fclose(p);
	//Closing the file
}

void AddItems(char* name, int mlay, ListMana* L)
//Function that adds to the quantity of asked dish
{
	int InList = 0;
	Mana* temp = L->head;
	//Setting temp to be the first dish
	if (mlay >= 0)
	{ //Checking if the added quantity is positive
		while (temp != NULL)
		{//A loop that searches the asked dish
			if (strcmp(temp->Name, name) == 0)
			{
				temp->Quantity += mlay;
				//Adding to asked dish the quantity requested
				InList = 1;
				printf("\n%d %s were added to the kitchen", mlay, temp->Name);
				//Remembering that we found asked dish
				break;
			}
			temp = temp->next;
		}
		if (!InList)
			//If the dish wasnt found ini list prints appropriate message
			printf("\nWe don't have %s, sorry!", name);
	}
	else
		printf("\nThe quantity of %s has to be more than 0.", name);
}

void FreeManotList(ListMana* L)
//Function that frees the name of the node and the node
{
	Mana* temp = L->head;
	while (L->head != NULL)
	{
		temp = L->head->next;
		free(L->head->Name);
		free(L->head);
		L->head = temp;
	}
}

void OrderItem(int TableN, char* name, int number, ListMana* L, Table* tables[])
{ // Function that orderes specific dish to requested table
	int InList = 0, i;
	Mana* temp = L->head, * temp_mana;
	//Defining temp to look at the first dish , and temp_mana is for the dish that we`ll add to the table
	if (TableN <= MAXTABLES && TableN >= 1)
	{ //Checking that the number of the table is between 1-50
		while (temp != NULL)
		{
			if (strcmp(temp->Name, name) == 0)
			{ //Finding requested dish in the menu
				if (temp->Quantity >= number && number >= 0)
				{ //Checking that the stock is larger than the number that was ordered, and that the number is positive
					if ((temp_mana = (Mana*)malloc(sizeof(Mana))) == NULL)
					{//Allocating memory to 'temp_mana' and checking if the allocation succeeded
						FreeManotList(L);
						for (i = 0; i < MAXTABLES; i++)
						{
							FreeManotList(&tables[i]->Tablefood);
							free(tables[i]);
							//Freeing all memorry and exits the program if fails
						}
						Error_Msg("\nThere was a problem allocationg memory.");
					}
					if ((temp_mana->Name = (char*)malloc((strlen(temp->Name) + 1) * sizeof(char))) == NULL)
					{ //Allocating memory to the name of 'temp_mana' and checking if the allocation succeeded
						FreeManotList(L);
						for (i = 0; i < MAXTABLES; i++)
						{
							FreeManotList(&tables[i]->Tablefood);
							free(tables[i]);
							//Freeing all memorry and exits the program if fails
						}
						Error_Msg("\nThere was a problem allocation the memory for the name");
					}
					temp->Quantity -= number;
					//Updating the quantity in stock
					strcpy(temp_mana->Name, name);
					temp_mana->Quantity = number;
					temp_mana->Price = temp->Price;
					//Updating the data of the new dish
					temp_mana->next = tables[TableN - 1]->Tablefood.head;
					tables[TableN - 1]->Tablefood.head = temp_mana;
					//Adding to thhe table`s list of food form the head
					printf("\n%d %s were added to table number %d", temp_mana->Quantity, temp->Name, TableN);
					InList = 1;
					break;
				}
				else
				{
					printf("\nSorry , There wasnt enough quantity of %s or problem with the quantity. dish wasnt ordered to the table.", name);
					break;
				}
				InList = 1;
			}
			temp = temp->next;
			//Advancing temp
		}
		if (!InList)
			printf("\nWe don't have %s, sorry!", name);
	}
}

void RemoveItem(int TableN, Table* tables[])
{ //Function that removes the last order from a table
	Mana* temp;
	if (tables[TableN - 1]->Tablefood.head != NULL)
	{ //Checking if the table has orders
		if (tables[TableN - 1]->IsReturned == 0)
		{ //Checking that table havent returned anything yet
			temp = tables[TableN - 1]->Tablefood.head->next;
			printf("\n%d %s was returned to the kitchen from table number %d", tables[TableN - 1]->Tablefood.head->Quantity, tables[TableN - 1]->Tablefood.head->Name, TableN);
			free(tables[TableN - 1]->Tablefood.head->Name);
			free(tables[TableN - 1]->Tablefood.head);
			//Freeing the name and then the dish
			tables[TableN - 1]->Tablefood.head = temp;
			//Updating head
			tables[TableN - 1]->IsReturned = 1;
			//Updating that this table has already returned food
		}
		else
			printf("\nSorry, table number %d has already returned 1 time", TableN);
	}
	else
		printf("\nTable number %d is not ordered yet", TableN);
}

void ResetTables(Table* tables[])
{ //Function that reset the tables 
	Table* temp;
	int i, j;
	for (i = 0; i < MAXTABLES; i++)
	{
		if ((temp = (Table*)malloc(sizeof(Table))) == NULL)
		{ //Allocating memory to temp and checking if the allocation succeeded
			for (j = 0; j < i; j++)
				free(tables[j]);
			Error_Msg("Reseting tables didnt work.");
		}
		temp->IsReturned = 0;
		temp->Tablefood.head = NULL;
		temp->Tablefood.Tail = NULL;
		//Defining head and tail to be NULL 
		tables[i] = temp;
		//putting temp into the list
	}
}

void RemoveTable(int TableN, Table* tables[])
{ //Function that frees the list of the food of a table and calculates and prints it's bill
	int TotalPrice = 0;
	Mana* temp = tables[TableN - 1]->Tablefood.head;
	printf("\n");
	if (temp != NULL)
	{ //Checking if the table has orders
		while (temp != NULL)
		{
			TotalPrice += (temp->Price) * (temp->Quantity);
			//Adding to TotalPrice the sum of the orders
			printf("%d %s", temp->Quantity, temp->Name);
			if (temp->next == NULL)
				printf(". ");
			else
				printf(", ");
			temp = temp->next;
			//Advancing temp
		}
		FreeManotList(&tables[TableN - 1]->Tablefood);
		//Freeing the table`s list of food
		printf("%d nis+%g nis for tips, please!", TotalPrice, (TotalPrice * 0.15));
	}
	else
		printf("The table number %d is not ordered yet", TableN);
}

void Error_Msg(char* msg)
{//Function that prints error message and finished the program
	printf("\n%s", msg);
	exit(1);
}
