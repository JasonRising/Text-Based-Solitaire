#ifndef _CARDS_H_
#define _CARDS_H_
#define HEART_SYMBOL "\xE2\x99\xA5"
#define DIAMOND_SYMBOL "\xE2\x99\xA6"
#define SPADE_SYMBOL "\xE2\x99\xA0"
#define CLUB_SYMBOL "\xE2\x99\xA3"
#define SIZE 52
#define INVALID 0
#define VALID 1

/* Suits enum */
typedef enum {
	HEARTS, DIAMONDS, SPADES, CLUBS
} suits;

/* Ranks enum */
typedef enum {
	TWO = 0, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
} ranks;

/* Card data type */
typedef struct {
	
	suits suit;
	ranks rank;
	unsigned int valid;
	
} card;

/* Deck of cards data type */
typedef struct {
	
	card cards[SIZE];
	
} deckOfCards;

int same_color(suits s1, suits s2);
int validSuit(int c);
void set_deck(deckOfCards* cards);
unsigned char get_rank(ranks x);
unsigned char get_suit(suits x);
void shuffle_deck(deckOfCards* cards, unsigned int howManyTimes);

#endif
