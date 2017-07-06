#include<stdio.h>
#include<stdlib.h>
#include "cards.h"

#define HOME 4
#define FIELD 7

typedef struct node node;
typedef node* nodeptr;

struct node
{
	card aCard;
	nodeptr next;
};

typedef struct
{
	nodeptr cardPile;
	nodeptr homePiles[HOME];
	nodeptr fieldPiles[FIELD];
} solitaireCards;

void print_directions(void);
int createGame(solitaireCards* c, const deckOfCards* d);

void print_gui(const nodeptr cP, const nodeptr* hP, const nodeptr* fP);
nodeptr createNode(card aCard);
void addNode(nodeptr* head, const nodeptr aNode, int front);
nodeptr removeNode(nodeptr* head, const card searchValue);
int searchForCardRank(nodeptr head, card* aCard);

void print_divider(FILE* fout, char ch, unsigned int total);
int get_choice(void);
int isValid(char c);
void printHand(nodeptr head);
int freeHand(nodeptr* head);
void destructor(nodeptr* cP, nodeptr* hP, nodeptr* fP);

int main(void)
{
	/* Solitaire */
	deckOfCards deck;
	solitaireCards gameCards = {0};
	
	/* Extra */
	int choice;
	char deleteChoice, addChoice;
	int valid;
	card tempCard;
	char tempRank;
	
	nodeptr previous, current;
	nodeptr temp;
	nodeptr swapNode;
	int i, x, cardIndex;
	
	/* Seed rand for shuffling */
	srand((unsigned)time(NULL));
	
	/* Set the deck of cards */
	set_deck(&deck);
	
	/* Shuffle the deck */
	shuffle_deck(&deck, 1);
	
	/* Create game */
	if(createGame(&gameCards, &deck))
	{
		/* Set current to the first card and previous to the second */
		current = gameCards.cardPile;
		previous = NULL;
	
		do
		{
			system("clear");
			print_divider(stdout, '-', 46);
			printf("%7cWelcome to the Game-of-Solitaire\n%12cDesigner: Jason Rising\n", ' ', ' ');
			print_divider(stdout, '-', 46);
			
			printf("(P)lay\n(D)irections\n(Q)uit\n\nChoice ==> ");
			choice = toupper(get_choice());
		
			if(choice == 'P')
			{
				do
				{
					tempRank = -1;
					
					do 
					{
						system("clear");
						print_gui(current, gameCards.homePiles, gameCards.fieldPiles);
						
						printf("Choices ==> ");
						deleteChoice = toupper(getchar());
						
						/* Determine whether to advance the card pile or continue getting input based on delete choice */
						if(deleteChoice == '\n')
						{
							/* Determine if no more cards can be advanced based on current */
							if(current != NULL)
							{
								/* Determine if one card is left based on current and previous */
								if(current->next != NULL || previous != NULL)
								{
									/* Move current and previous along */
									previous = current;
									current = current->next;
									
									/* Determine current and prvious based on current */
									if(current == NULL)
									{	
										/* Reset current to the beginning and previous to NULL */
										current = gameCards.cardPile;
										previous = NULL;
								
									}
								}
							}
						}
						else if(deleteChoice != 'Q')
						{
							/* Assign the position to add the card(s) to */
							addChoice = toupper(get_choice());
							
							/* Determine if multiple cards are to be moved based on add choice */
							if(addChoice == 'M')
							{
								printf("Rank and stack ==> ");
								tempRank = toupper(getchar());
								
								addChoice = toupper(get_choice());
							}
						}
						
					}while(addChoice == deleteChoice || addChoice == 'P' || deleteChoice == '\n' || deleteChoice != 'Q' && (!isValid(deleteChoice) || !isValid(addChoice)) ||
					      (tempRank != -1 && !validSuit(tempRank)));
					
					if(tempRank != -1) tempCard.rank = get_the_rank(tempRank);
					
					valid = 1;
				
					if(deleteChoice != 'Q')
					{
						if(tempRank != -1 || (deleteChoice >= '1' && deleteChoice <= '7'))
						{
							if(deleteChoice >= '1' && deleteChoice <= '7')
							{
								if(gameCards.fieldPiles[deleteChoice - '1'] == NULL) valid = 0;
								else if(tempRank != -1)
								{
									if(!searchForCard(gameCards.fieldPiles[deleteChoice - '1'], &tempCard)) valid = 0;
								}
								else tempCard = gameCards.fieldPiles[deleteChoice - '1']->aCard;
							}
							else valid = 0;
						}
						else if(deleteChoice == 'P')
						{
							if(gameCards.cardPile == NULL) valid = 0;
							else tempCard = current->aCard;
						}
						else if(deleteChoice >= 'A' && deleteChoice <= 'D')
						{
							if(gameCards.homePiles[deleteChoice - 'A'] == NULL) valid = 0;
							else tempCard = gameCards.homePiles[deleteChoice - 'A']->aCard;
						}
	
						if(valid)
						{
							
							if(addChoice >= 'A' && addChoice <= 'D')
							{
								if(gameCards.homePiles[addChoice - 'A'] == NULL)
								{
									if(tempCard.rank != ACE) valid = 0;
								}
								else if(gameCards.homePiles[addChoice - 'A']->aCard.suit != tempCard.suit) valid = 0;
								else
								{
									if(gameCards.homePiles[addChoice - 'A']->aCard.rank == ACE)
									{
										if(tempCard.rank != TWO) valid = 0;
									}
									else if(tempCard.rank - 1 != gameCards.homePiles[addChoice - 'A']->aCard.rank) valid = 0;
								}
							}
							else
							{
								if(gameCards.fieldPiles[addChoice - '1'] == NULL)
								{
									if(tempCard.rank != KING) valid = 0;
								}
								else if(gameCards.fieldPiles[addChoice - '1']->aCard.rank == ACE || 
									same_color(gameCards.fieldPiles[addChoice - '1']->aCard.suit, tempCard.suit) || 
									(gameCards.fieldPiles[addChoice - '1']->aCard.rank == TWO && tempCard.rank != ACE) || 
									(gameCards.fieldPiles[addChoice - '1']->aCard.rank != TWO && tempCard.rank == ACE) ||
									(tempCard.rank != ACE && tempCard.rank + 1 != gameCards.fieldPiles[addChoice - '1']->aCard.rank))
								{
									valid = 0;
								}
							}
						}
						
						if(valid)
						{
							/* Determine  */
							if(deleteChoice == 'P')
							{
								swapNode = removeNode(&gameCards.cardPile, tempCard);
								
								if(previous == NULL)
								{
									current = gameCards.cardPile;
								}
								else
								{
									current = previous;
									
									if(current == gameCards.cardPile)
									{
										previous = NULL;
									}
									else
									{
										previous = gameCards.cardPile;
										while(previous->next != current) previous = previous->next;
									}	
								}
								
								if(addChoice >= 'A' && addChoice <= 'D')
								{
									addNode(&gameCards.homePiles[addChoice - 'A'], swapNode, 1);
								}
								else	
								{
									addNode(&gameCards.fieldPiles[addChoice - '1'], swapNode, 1);
								}
							}
							else if(deleteChoice >= 'A' && deleteChoice <= 'D')
							{
								swapNode = removeNode(&gameCards.homePiles[deleteChoice - 'A'], tempCard);
								
								if(addChoice >= 'A' && addChoice <= 'D')
								{
									addNode(&gameCards.homePiles[addChoice - 'A'], swapNode, 1);
								}
								else	
								{
									addNode(&gameCards.fieldPiles[addChoice - '1'], swapNode, 1);
								}
							}
							else
							{
								if(tempRank != -1)
								{
									swapNode = gameCards.fieldPiles[deleteChoice - '1'];
									
									while(swapNode != NULL && (swapNode->aCard.suit != tempCard.suit || swapNode->aCard.rank != tempCard.rank))
									{
										swapNode = swapNode->next;
									}
									
									temp = gameCards.fieldPiles[deleteChoice - '1'];
									gameCards.fieldPiles[deleteChoice - '1'] = swapNode->next;
									swapNode->next = NULL;
								}
								else
								{
									swapNode = removeNode(&gameCards.fieldPiles[deleteChoice - '1'], tempCard);
								}
								
								if(gameCards.fieldPiles[deleteChoice - '1'] != NULL && gameCards.fieldPiles[deleteChoice - '1']->aCard.valid == INVALID)
								{
									gameCards.fieldPiles[deleteChoice - '1']->aCard.valid = VALID;
								}
								
								if(addChoice >= 'A' && addChoice <= 'D')
								{
									addNode(&gameCards.homePiles[addChoice - 'A'], swapNode, 1);
								}
								else
								{
									addNode(&gameCards.fieldPiles[addChoice - '1'], swapNode, 1);
									
									if(tempRank != -1)
									{
										gameCards.fieldPiles[addChoice - '1'] = temp;
										temp = NULL;
									}
								}
							}
						}
					}
					
				}while(deleteChoice != 'Q');
			}
			else if(choice == 'D')
			{
				system("clear");
				print_directions();
				
				get_choice();
			}
			
		}while(choice != 'Q');
	}
	
	destructor(&gameCards.cardPile, gameCards.homePiles, gameCards.fieldPiles);

	return 0;
}

void print_directions(void)
{
	printf("The Game-of-Solitaire\n\n\tThe goal of the game is to move all of the cards from the field to the\nhome piles marked A-D in ascending order starting with ACE then TWO\ns and so forth. The field decks marked 1-7 are used create lists in descending order starting from KING then QUEEN and so forth. Any empty spaces marked * is open to place a KING down and start a list.\n\nEnter key: Move through home pile\n\nTo move cards around, type two numbers: [Pile to remove][Pile to add to]\nTo move multiple Cards: [Pile to remove]m. Then type[Card rank of the pile][Pile to add to]\n");
}

void print_divider(FILE* fout, char ch, unsigned int total)
{
	if(fout != NULL)
	{
		while(total-- > 0) fputc(ch, fout);
		fputc('\n', fout);
	}
}

void print_gui(const nodeptr cP, const nodeptr* hP, const nodeptr* fP)
{
	nodeptr walker;
	
	int index, iterations, currentPile, largestCount;
	int counts[7] = {0};
	
	if(cP != NULL && hP != NULL && fP != NULL)
	{
		print_divider(stdout, '-', 43);
		
		printf("|%2cP)%16cA)%4cB)%4cC)%4cD) |\n|", '(', '(', '(', '(', '(');
		
		if(cP != NULL) printf("[%c %c]", get_rank(cP->aCard.rank), get_suit(cP->aCard.suit));
		else printf("%3c   ", '*');
		printf("%12c", ' ');
				
		for(index = 0; index < HOME; index++)
		{
			if(hP[index] == NULL)
			{
				printf("%4c", '*');
				if(index < HOME-1) printf("%2c", ' ');
				else printf("%3c", '|');
			}
			else
			{
				printf(" [%c %c]", get_rank(hP[index]->aCard.rank), get_suit(hP[index]->aCard.suit));
				
				if(index == HOME-1) putchar('|');
			}
		}
		
		printf("\n|%42c\n|%2c1)%4c2)%4c3)%4c4)%4c5)%4c6)%4c7) |\n", '|', '(', '(', '(', '(', '(', '(', '(');
		
		largestCount = 0;
		
		/* Accumulate the counts in each deck */
		for(index = 0; index < FIELD; index++)
		{
			/* Assign the current pile to traverse */
			walker = fP[index];
			
			while(walker != NULL)
			{
				counts[index]++;
				walker = walker->next;
			}
			
			if(counts[index] > largestCount)
			{
				largestCount = counts[index];
			}
		}
		
		putchar('|');
		
		/* Check each pile for NULL to print * */
		for(index = 0; index < FIELD; index++)
		{
			walker = fP[index];
				
			if(walker != NULL)
			{
				while(walker->next != NULL) walker = walker->next;
			}
			
			if(walker == NULL) printf("%3c  ", '*');
			else if(walker->aCard.valid == VALID) printf("[%c %c]", get_rank(walker->aCard.rank), get_suit(walker->aCard.suit));
			else printf("-----");
			
			if(index < FIELD-1) putchar(' ');
			
			if(walker != NULL) counts[index]--;
		}

		printf("|\n");
		
		for(iterations = 1; iterations < largestCount; iterations++)
		{
			putchar('|');
			
			for(currentPile = 0; currentPile < FIELD; currentPile++)
			{
				walker = fP[currentPile];
				
				for(index = 1; index < counts[currentPile]; index++)
				{
					walker = walker->next;
				}
				
				if(!counts[currentPile]) printf("%6c", ' ');
				else
				{
					if(walker->aCard.valid) printf("[%c %c]", get_rank(walker->aCard.rank), get_suit(walker->aCard.suit));
					else printf("-----");
					
					if(currentPile < FIELD-1) putchar(' ');
					
					counts[currentPile]--;
				}
			}
			
			printf("|\n");
		}
		
		print_divider(stdout, '-', 43);
	}
	else
	{
		printf("--Error--");
	}
	
	putchar('\n');
}

int get_choice(void)
{
	int choice = getchar();
	
	while(getchar() != '\n');
	
	return choice;
}

nodeptr createNode(card aCard)
{
	nodeptr temp;
	
	if((temp = (nodeptr)malloc(sizeof(node))) != NULL);
	{
		temp->aCard = aCard;
		temp->next = NULL;
	}
	
	return temp;
}

int createGame(solitaireCards* c, const deckOfCards* d)
{
	int i, x, cardIndex;
	int valid;
	nodeptr temp;
	
	valid = 1;
	
	cardIndex = 0;
	
	for(i = 0; i < FIELD && valid; i++)
	{
		for(x = i; x < FIELD && valid; x++)
		{
			temp = createNode(d->cards[cardIndex++]);
			
			if(temp != NULL)
			{
				if(x != i) temp->aCard.valid = 0;
			
				addNode(&c->fieldPiles[x], temp, 1);
			}
			else
			{
				valid = 0;
			}
		}
	}
	
	while(valid && cardIndex < SIZE)
	{
		temp = createNode(d->cards[cardIndex++]);
		
		if(temp != NULL)
		{
			addNode(&c->cardPile, temp, 0);
		}
		else
		{
			valid = 0;
		}
	}
	
	return valid;
}

/* Inserts a node into a linked list */
void addNode(nodeptr* head, const nodeptr aNode, int front)
{
	nodeptr walker;
	
	/* Determine the insertion point of the node based on head */
	if(*head == NULL)
	{
		*head = aNode;
	}
	else
	{
		/* Determine insertion of node based on front */
		if(front)
		{
			/* Insert to the front */
			aNode->next = *head;
			*head = aNode;
		}
		else
		{
			/* Initialize walker */
			walker = *head;
			
			/* Traverse to the end of the list */
			while(walker->next != NULL) walker = walker->next;
			
			/* Assign the last node's link to the insertion node */
			walker->next = aNode;
		}
	}
}

/* Removes a node from a linked list and returns a pointer to it for insertion into another deck */
nodeptr removeNode(nodeptr* head, const card searchValue)
{
	nodeptr previous = NULL;
	nodeptr retValue = *head;	
	
	/* Determine the return value based on retValue */
	if(retValue != NULL)
	{
		/* Traverse the linked list to find the node to remove */
		while(retValue != NULL && (retValue->aCard.rank != searchValue.rank || retValue->aCard.suit != searchValue.suit))
		{
			/* Move the pointers along */
			previous = retValue;
			retValue = retValue->next;
		}
		
		/* Determine the links in the list based on the ret value */
		if(retValue != NULL)
		{
			/* Determine the links of the list based on the ret value */
			if(retValue == *head)
			{
				/* Move the head of the list along */
				*head = (*head)->next;
				retValue->next = NULL;
			}
			else
			{
				/* Reconnect the nodal links */
				previous->next = retValue->next;
				retValue->next = NULL;
			}
		}
	}
	
	return retValue;
}

int searchForCard(nodeptr head, card* aCard)
{
	int found = 0;
	
	while(head != NULL && !found)
	{
		if(head->aCard.rank == aCard->rank)
		{
			found = 1;
			
			aCard->suit = head->aCard.suit;
			head->aCard.valid = 1;
		}
		else
		{
			head = head->next;
		}
	}
	
	return found;
}

int isValid(char c)
{
	int valid = 1;
	
	if(c != 'P' && !(c >= '1' && c <= '7') && !(c >= 'A' && c <= 'D')) valid = 0;
	   
	return valid;
}

void printHand(nodeptr head)
{
	while(head != NULL)
	{
		printf("[R: %c  S: %c] -> ", get_rank(head->aCard.rank), get_suit(head->aCard.suit));
		
		head = head->next;
	}
	
	printf("NULL\n");
}

int freeHand(nodeptr* head)
{
	nodeptr temp;
	int count;
	
	count = 0;
	
	while(*head != NULL)
	{
		temp = *head;
		*head = temp->next;
		temp->next = NULL;
		
		free(temp);
		
		count++;
	}
	
	return count;
}

void destructor(nodeptr* cP, nodeptr* hP, nodeptr* fP)
{
	int i, count = 0;
	
	count += freeHand(cP);
	
	for(i = 0; i < FIELD; i++)
	{
		count += freeHand(&fP[i]);
	}
	
	for(i = 0; i < HOME; i++)
	{
		count += freeHand(&hP[i]);
	}
	
	printf("Total Destructed: %d\n", count);
}
