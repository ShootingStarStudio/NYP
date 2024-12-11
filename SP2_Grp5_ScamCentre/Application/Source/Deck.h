#pragma once
#include "Card.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Deck
{
public:
	Deck();
	~Deck();

	void resetopen(void);
	void resethand(vector <Card>& hand);
	void addcard(vector <Card>& hand);
	int valuecount(vector <Card>& hand);

	void printdeck(vector <Card>& hand);

	vector <Card> opendeck;
	vector <Card> dealerhand;
	vector <Card> playerhand;

private:
	//default deck
	Card cardlist[52] = {	
		Card(1,3),Card(2,3),Card(3,3),Card(4,3),Card(5,3),Card(6,3),Card(7,3),Card(8,3),Card(9,3),Card(10,3),Card(10,3),Card(10,3),Card(10,3), 
		Card(1,4),Card(2,4),Card(3,4),Card(4,4),Card(5,4),Card(6,4),Card(7,4),Card(8,4),Card(9,4),Card(10,4),Card(10,4),Card(10,4),Card(10,4), 
		Card(1,5),Card(2,5),Card(3,5),Card(4,5),Card(5,5),Card(6,5),Card(7,5),Card(8,5),Card(9,5),Card(10,5),Card(10,5),Card(10,5),Card(10,5),
		Card(1,6),Card(2,6),Card(3,6),Card(4,6),Card(5,6),Card(6,6),Card(7,6),Card(8,6),Card(9,6),Card(10,6),Card(10,6),Card(10,6),Card(10,6),
	};
	
};

