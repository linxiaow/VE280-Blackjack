#include "deck.h"
#include "hand.h"
#include "card.h"
#include"rand.h"
#include"player.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<cassert>
using namespace std;
void randShuffle(Deck &deck) {
//EFFECT: shuffling the deck and anounce
//MODIFFIES: deck
	for (int i = 0; i < 7; i++)
		deck.shuffle(get_cut());
}
void inputShuffle(Deck &deck, string filename) {
//EFFECT: shuffling the deck according to the input file and anounce
//MODIFFIES: deck
	ifstream iFile;
	iFile.open(filename);
	string inputShuff;
	getline(iFile, inputShuff);
	iFile.close();

	istringstream iData;
	iData.str(inputShuff);
	int cutNumber;
	while(iData>>cutNumber)
		deck.shuffle(cutNumber);
}
Card dealing(Hand &thisMan, Deck &deck) {
//EFFECT: return the cardDealt
//MODIFFIES: thisMan and deck
	try { 
	Card thisCard = deck.deal();
	thisMan.addCard(thisCard);	
	return thisCard; 
	}
	catch (DeckEmpty empty) { assert(0); }
}
bool isNatural21(const Hand &thisHand, const int bet, int &bankroll) {
//EFFECTS: return true if the player hits a natural 21
//and display the win information
//false otherwise
//MODIFIES: bankroll
	if (thisHand.handValue().count == 21) {
		cout << "# Player dealt natural 21\n";
		bankroll += bet * 3 / 2;
		return true;
	}
	else return false;
}
bool bust(const Hand &thisHand) {
//EFFECTS: return true if the one man busts
//false otherwise
	if (thisHand.handValue().count > 21) 
		return true;
	else return false;
}
void judgeWin(const Hand player, const Hand dealer, const bool playerBust, 
	const bool dealerBust, int &bankroll,const int bet) {
//EFFECTS: display the wining result
//MODIFIES: bankroll
	if (playerBust){
		cout << "# Player busts\n";
		bankroll -= bet;
		return;
	}
	if (dealerBust){
		cout << "# Dealer busts\n";
		bankroll += bet;
		return;
	}
	if (player.handValue().count > dealer.handValue().count) {
		cout << "# Player wins\n";
		bankroll += bet;
		return;
	}
	if (dealer.handValue().count > player.handValue().count) {
		cout << "# Dealer wins\n";
		bankroll -= bet;
		return;
	}
	if (dealer.handValue().count == player.handValue().count) {
		cout << "# Push\n";
		return;
	}
}

int main(int argc, char *argv[]) {
	int bankroll = atoi(argv[1]);
	int minBet = atoi(argv[2]);
	int hands = atoi(argv[3]);
	string playerType = argv[4];
	Deck deckPlaying;
	Player *playing;
	if (playerType == "simple")
		playing = get_Simple();
	else
		playing = get_Counting();

	if (argc > 5) {
		//there is input file
		string filename = argv[5];
		playing->shuffled();
		inputShuffle(deckPlaying, filename);
		//shuffle with input numbers
	}
	else {
		playing->shuffled();
		randShuffle(deckPlaying);//random shuffle
	}

	int thishand = 0;
	while (bankroll>=minBet && thishand < hands) {
	// if the bankroll is enough 
		thishand++;

		Hand player; Hand dealer;//anounce an hand of player and dealer;
		bool dealerBust = false; bool playerBust = false;
		//initialize the bust to be all false;

		cout << "# Hand " << thishand << " bankroll " << bankroll << endl;
		if (deckPlaying.cardsLeft() < 20) {
		//the cards are fewer than 20
			playing->shuffled();
			randShuffle(deckPlaying);//shuffing the deck
		}

		int bet = playing->bet(bankroll, minBet);
		cout << "# Player bets " << bet << endl;

		cout << "Player dealt ";
		playing->expose(dealing(player, deckPlaying));
		//modiffy as well as expose

		cout << "Dealer dealt ";
		Card dealerCard = dealing(dealer, deckPlaying);
		playing->expose(dealerCard);

		cout << "Player dealt ";
		playing->expose(dealing(player, deckPlaying));

		Card holeCard = dealing(dealer, deckPlaying);
		//this card is face-down
		
		if (isNatural21(player, bet, bankroll))
		// if it is natural 21,end and begin next round
			continue;
		

		while (1){
			while (playing->draw(dealerCard, player)) {
				//player hits
				cout << "Player dealt ";
				playing->expose(dealing(player, deckPlaying));
				//modiffy as well as expose
			}
			cout << "Player's total is " << player.handValue().count << endl;
			if (bust(player)) {
				playerBust = true;
				break;//go to next hand if player busts
			}

			cout << "Dealer's hole card is ";
			playing->expose(holeCard);//expose the hole card
			while (dealer.handValue().count < 17) {
				//dealer hit
				cout << "Dealer dealt ";
				playing->expose(dealing(dealer, deckPlaying));
			}
			cout << "Dealer's total is " << dealer.handValue().count << endl;
			if (bust(dealer)) { 
				dealerBust = true;
				break;//go to next hand if dealer busts
			}

			break;
		}


		 judgeWin(player, dealer, playerBust, dealerBust, bankroll, bet);
		 player.discardAll();
		 dealer.discardAll();

	}

	cout << "# Player has " << bankroll
		<< " after " << thishand << " hands\n";

	delete playing;

	return 0;
}