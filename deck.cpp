#include "deck.h"
#include <iostream>
using namespace std;
Deck::Deck() {
// EFFECTS: constructs a "newly opened" deck of cards.  first the
// spades from 2-A, then the hearts, then the clubs, then the
// diamonds.  The first card dealt should be the 2 of Spades.
	reset();
}

void Deck::reset() {
// EFFECTS: resets the deck to the state of a "newly opened" deck
// of cards:
	int number = 0;
	int type = 0;
	int count = 0;
	for (count = 0; count<DeckSize; count++) {
		if (number == SPOT_SIZE) number = 0;
		type = count / SPOT_SIZE;
		deck[count].spot = (Spot)number;
		deck[count].suit = (Suit)type;
		number++;
	}
	next = 0;
	return;
}

void Deck::shuffle(int n) {
// REQUIRES: n is between 0 and 52, inclusive.

// MODIFIES: this

// EFFECTS: cut the deck into two segments: the first n cards,
// called the "left", and the rest called the "right".  Note that
// either right or left might be empty.  Then, rearrange the deck
// to be the first card of the right, then the first card of the
// left, the 2nd of right, the 2nd of left, and so on.  Once one
// side is exhausted, fill in the remainder of the deck with the
// cards remaining in the other side.  Finally, make the first
// card in this shuffled deck the next card to deal.  For example,
// shuffle(26) on a newly-reset() deck results in: 2-clubs,
// 2-spades, 3-clubs, 3-spades ... A-diamonds, A-hearts.
//
// Note: if shuffle is called on a deck that has already had some
// cards dealt, those cards should first be restored to the deck
// in the order in which they were dealt, preserving the most
// recent post-shuffled/post-reset state.

	cout << "cut at " << n << endl;
	int times = n <= DeckSize - n ? n : DeckSize - n;//give the times to shuffle
	Card leftCard[DeckSize];
	Card rightCard[DeckSize];
	for (int i = 0; i < n; i++)
		leftCard[i] = deck[i];
	for (int i = 0; i < DeckSize - n; i++)
		rightCard[i] = deck[n + i];

	for (int i = 0; i < times; i++) {
		deck[2*i] = rightCard[i];
		deck[2 * i + 1] = leftCard[i];
	}
	
	if (times == n) {//if the left card exausts first
		for (int i = 0; i < DeckSize-n-times; i++)
			deck[2*times+i] = rightCard[times + i];
	}

	if (times == DeckSize-n) {//if the right card exausts first
		for (int i = 0; i < n-times; i++)
			deck[2*times+i] = leftCard[times + i];
	}
	next = 0;
}

Card Deck::deal() {
	if (cardsLeft() <= 0){
		DeckEmpty emptyDeck;
		throw emptyDeck;
	}
	next++;
	return deck[next-1];
}

int Deck::cardsLeft() {
	return DeckSize - next;
}



