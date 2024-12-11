#pragma once
#include "Entity.h"
class Golem :public Entity {
public:
	Golem();
	~Golem();
	void move(int);
};