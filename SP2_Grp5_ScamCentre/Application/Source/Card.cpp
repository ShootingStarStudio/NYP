#include "Card.h"


Card::Card() {
	Value = 0;
	Suit = 0;
}

Card::Card(int V, char S) {
	Value = V;
	Suit = S;
}
Card::~Card() {

}

char Card::getsuit(void) {
	return Suit;
}
void Card::setsuit(char newsuit){
	Suit = newsuit;
}

int Card::getvalue(void) {
	return Value;
}
void Card::setvalue(int newvalue) {
	Value = newvalue;
}