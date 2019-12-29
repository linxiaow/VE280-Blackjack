#include "hand.h"
Hand::Hand() {
// EFFECTS: establishes an empty blackjack hand.
	discardAll();
}

void Hand::discardAll() {
// MODIFIES: this
// EFFECTS: discards any cards presently held, restoring the state
// of the hand to that of an empty blackjack hand.
	curValue.count = 0;
	curValue.soft = false;//No ACE, hardCount
	return;
}

void Hand::addCard(Card c) {
// MODIFIES: this
// EFFECTS: adds the card "c" to those presently held.
	if (c.spot <= 8) 
		curValue.count += c.spot + 2;
	if (c.spot <=11 && c.spot>=9)
		curValue.count += 10;
	if (c.spot == ACE) {
		if (curValue.soft) {//there has been an ACE before
			curValue.count += 1;
		}
		else if (curValue.count + 11 <= 21) {
		//no ACE before, and soft count is OK
			curValue.count += 11;//it is soft count                
			curValue.soft = true;
		}
		else {
		//no ACE before, and has to be a hard count
			curValue.count += 1;
			curValue.soft = false;//hard count ACE == 1
		}
	}

	if (curValue.count > 21 && curValue.soft) {
	//if the value exceed 21 and ACE is hard count
		curValue.count -= 10;
		curValue.soft = false; //turn to hard count
	}
	return;
}

HandValue Hand::handValue() const{
// EFFECTS: returns the present value of the blackjack hand.  The
// count field is the highest blackjack total possible without
// going over 21.  The soft field should be true if and only if at
// least one ACE is present, and its value is counted as 11 rather
// than 1.  If the hand is over 21, any value over 21 may be returned.
//
// Note: the const qualifier at the end of handValue means that
// you are not allowed to change any member variables inside
// handValue. Adding this prevents the accidental change by you.
	return curValue;
}