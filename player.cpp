#include "player.h"
#include <iostream>
using namespace std;
class SimplePlayer : public Player 
{
public:
	virtual int bet(unsigned int bankroll,
		unsigned int minimum);
	// REQUIRES: bankroll >= minimum
	// EFFECTS: returns the player's bet, between minimum and bankroll 
	// inclusive

	virtual void expose(Card c);

	bool draw(Card dealer,// Dealer's "up card"
		const Hand &player); // Player's current hand
								 // EFFECTS: returns true if the player wishes to be dealt another
								 // card, false otherwise.
	virtual void shuffled();
};

class CountingPlayer : public SimplePlayer
{
private:
	int count;
public:
	CountingPlayer();
	//constructor;

	virtual int bet(unsigned int bankroll,
		unsigned int minimum);
	// REQUIRES: bankroll >= minimum
	// EFFECTS: returns the player's bet, between minimum and bankroll 
	// inclusive

	virtual void expose(Card c);
	// EFFECTS: allows the player to "see" the newly-exposed card c.
	// MODDIFIES: the number of count;
	// For example, each card that is dealt "face up" is expose()d.
	// Likewise, if the dealer must show his "hole card", it is also
	// expose()d.  Note: not all cards dealt are expose()d---if the
	// player goes over 21 or is dealt a natural 21, the dealer need
	// not expose his hole card.

	virtual void shuffled();
	// EFFECTS: tells the player that the deck has been re-shuffled.

};

int SimplePlayer::bet(unsigned int bankroll,
	unsigned int minimum) {
	return minimum;
}

bool SimplePlayer::draw(Card dealer,// Dealer's "up card"
	const Hand &player) {  // Player's current hand
// EFFECTS: returns true if the player wishes to be dealt another
// card, false otherwise.
	int counting = player.handValue().count;
	int dealerCounting = dealer.spot + 2;
	//make the number equal to our familiar type

	if (!player.handValue().soft){//hard count
		if (counting <= 11) return true;//hits
		if (counting == 12){
			if (dealerCounting <= 6 && dealerCounting >= 4) {
			//the dealer shows 4, 5, 6
				return false;//he stands
			}
			else return true;//he hits
		}
		if (counting >= 13 && counting <= 16) {
			if (dealerCounting <= 6 && dealerCounting >= 2) {
				//the dealer shows 4, 5, 6
				return false;//he stands
			}
			else return true;//he hits
		}
		if (counting >= 17) return false;//he stands
	}

	if (player.handValue().soft) {//soft count
		if (counting <= 17) return true;//hits
		if (counting == 18) {
			if (dealerCounting == 2 || dealerCounting == 7 
				|| dealerCounting == 8) {
				//the dealer shows 2, 7, 8
				return false;//he stands
			}
			else return true;//he hits
		}
		if (counting >= 19) return false;//he stands
	}
	return false;
}

void SimplePlayer::expose(Card c) {
	cout << SpotNames[c.spot]
		<< " of " << SuitNames[c.suit] << endl;
}

void SimplePlayer::shuffled() { 
	cout << "# Shuffling the deck\n"; 
	return; 
}

CountingPlayer::CountingPlayer(): count(0) {}//如果有shuffled可以不用这个

int CountingPlayer::bet(unsigned int bankroll,
	unsigned int minimum){
// REQUIRES: bankroll >= minimum
// EFFECTS: returns the player's bet, between minimum and bankroll 
// inclusive
	if (count >= 2 && bankroll >= 2 * minimum)
		return 2 * minimum;
	else return minimum;
}

void CountingPlayer::expose(Card c) {
	cout << SpotNames[c.spot] 
		<< " of " << SuitNames[c.suit]<< endl;
	int number = c.spot + 2;
	if (number >= 2 && number <= 6) count++;
	if (number >= 10 && number <= 14) count--;
	return;
}

void CountingPlayer::shuffled() {
// EFFECTS: tells the player that the deck has been re-shuffled.
// MODDIFIES: number of count

	//here is the cout
	cout << "# Shuffling the deck\n";
	count = 0;
	return;
}

Player *get_Simple() {
// EFFECTS: returns a pointer to a "simple player", as defined by the
// project specification
	SimplePlayer *simple = new SimplePlayer;
	return simple;
}
Player *get_Counting() {
// EFFECTS: returns a pointer to a "counting player", as defined by
// the project specification.
	CountingPlayer *counting = new CountingPlayer;
	return counting;
}
