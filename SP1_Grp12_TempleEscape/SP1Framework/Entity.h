#pragma once
#include "Position.h"
#include <string>
#include <Windows.h>

class Entity {
public:
	Entity();
	~Entity();
	
	virtual void move(int) = 0;

	int get_hp(void);
	void set_hp(int);

	int get_dmg(void);
	void set_dmg(int);

	int get_def(void);
	void set_def(int);

	void setPos(int, int);
	int get_posX(void);
	int get_posY(void);

	std::string get_face(void);

	WORD get_colour(void);

	virtual int get_patrol_dir(void);

protected:
	Position EntityPos;
	int hp, dmg, def;
	std::string face;
	WORD colour;
};