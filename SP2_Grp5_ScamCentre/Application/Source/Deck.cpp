#include "Deck.h"

Deck::Deck() {
	srand(time(NULL));
}
Deck::~Deck() {

}

void Deck::resetopen(void) {
	opendeck.clear();
	for (size_t i = 0; i < size(cardlist); i++) {
		opendeck.push_back(cardlist[i]);
	}
	random_shuffle(opendeck.begin(), opendeck.end());
}

void Deck::resethand(vector <Card>& hand) {
	hand.clear();
}

void Deck::addcard(vector <Card>& hand) {
	hand.push_back(opendeck.back());
	opendeck.pop_back();

}
int Deck::valuecount(vector <Card>& hand) {
	int totalvalue = 0;
	for (vector<Card>::iterator it = hand.begin(); it != hand.end(); ++it) {
		totalvalue += it->getvalue();
	}
	return totalvalue;
}

void Deck::printdeck(vector <Card>& hand) {
	for (size_t i = 0; i < size(hand);i++) {
		std::cout << hand[i].getsuit() << ", " << hand[i].getvalue() << std::endl;
	}
}