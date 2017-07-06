#include "cards.h"

/* Initializes a deck of cards passed in */
void set_deck(deckOfCards* cards)
{
	int count;
	ranks rank;
	suits suit;
	
	/* Initialize the count */
	count = 0;
	
	/* Assign the deck 52 cards */
	for(suit = HEARTS; suit <= CLUBS; suit++)
	{
		/* Assign each card the current suit and a rank */
		for(rank = TWO; rank <= ACE; rank++)
		{
			cards->cards[count].suit = suit;
			cards->cards[count].rank = rank;
			cards->cards[count++].valid = VALID;
		}
	}
}

/* Determines if two suits are the same color */
int same_color(suits s1, suits s2)
{
	int same = 0;
	
	if(s1 == s2) same = 1;
	else if(s1 == HEARTS || s1 == DIAMONDS)
	{
		if(s2 == HEARTS || s2 == DIAMONDS) same = 1;
	}
	else if(s2 == SPADES || s2 == CLUBS) same = 1;
	
	return same;
}

/* Determines if the value passed in is a valid suit, 0-13 or the characters representing them */
int validSuit(int c)
{
	int valid = 0;
	
	/* Determine if the suit is valid based on c */
	if((c >= TWO && c <= ACE) || (c >= '2' && c <= '9') || c == 'T' || c == 'J' || c == 'Q' || c == 'K' || c == 'A') valid = 1;
	
	return valid;
}

/* Determines the rank of the card passed in based on x. Default is ACE */
unsigned char get_rank(ranks x)
{
	unsigned char rank;
	
	/* Determine the rank of the number passed in based on x */
	switch(x)
	{
		case TWO:
			rank = '2';
			break;
		case THREE:
			rank = '3';
			break;
		case FOUR:
			rank = '4';
			break;
		case FIVE:
			rank = '5';
			break;
		case SIX:
			rank = '6';
			break;
		case SEVEN:
			rank = '7';
			break;
		case EIGHT:
			rank = '8';
			break;
		case NINE:
			rank = '9';
			break;
		case TEN:
			rank = 'T';
			break;
		case JACK:
			rank = 'J';
			break;
		case QUEEN:
			rank = 'Q';
			break;
		case KING:
			rank = 'K';
			break;
		default:
			rank = 'A';
			break;
	}
	
	return rank;
}

/* Determines the rank of the card passed in based on x. Default is ACE */
ranks get_the_rank(unsigned char x)
{
	ranks rank;
	
	/* Determine the rank of the number passed in based on x */
	switch(x)
	{
		case '2':
			rank = TWO;
			break;
		case '3':
			rank = THREE;
			break;
		case '4':
			rank = FOUR;
			break;
		case '5':
			rank = FIVE;
			break;
		case '6':
			rank = SIX;
			break;
		case '7':
			rank = SEVEN;
			break;
		case '8':
			rank = EIGHT;
			break;
		case '9':
			rank = NINE;
			break;
		case 'T':
			rank = TEN;
			break;
		case 'J':
			rank = JACK;
			break;
		case 'Q':
			rank = QUEEN;
			break;
		case 'K':
			rank = KING;
			break;
		default:
			rank = ACE;
			break;
	}
	
	return rank;
}

/* Determines the suit of a number passed in based on x. Default is CLUBS*/
unsigned char get_suit(suits x)
{
	unsigned char suit;
	
	/* Determine the suit of the number passed in based on x */
	switch(x)
	{
		case HEARTS:
			suit = 'H';
			break;
		case DIAMONDS:
			suit = 'D';
			break;
		case SPADES:
			suit = 'S';
			break;
		default:
			suit = 'C';
			break;
	}
	
	return suit;
}

/* Randomize the deck of cards passed in */
void shuffle_deck(deckOfCards* cards, unsigned int howManyTimes)
{
	int i, r, t;
	card temp;
	
	/* Shuffle the deck of cards based on  */
	for(t = 1; t <= howManyTimes; t++)
	{	
		/* Shuffle the deck of cards */
		for(i = 0; i < SIZE; i++)
		{
			/* Get a random position */
			r = rand() % SIZE;
			
			/* Swap the two cards */
			temp = cards->cards[i];
			cards->cards[i] = cards->cards[r];
			cards->cards[r] = temp;
		}
	}
}
