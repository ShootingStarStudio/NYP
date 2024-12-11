#include "Entity.h"
#include <Windows.h>
#include <stdlib.h>

Entity::Entity() {
	hp = 0, dmg = 0, def = 0, face = "", colour = 0x0F;
}

Entity::~Entity() {

}

int Entity::get_hp(void) {
	return hp;
}
void Entity::set_hp(int a) {
	hp = a;
}

int Entity::get_dmg(void) {
	return dmg;
}
void Entity::set_dmg(int a) {
	dmg = a;
}

int Entity::get_def(void) {
	return def;
}
void Entity::set_def(int a) {
	def = a;
}

int Entity::get_posX(void) {
	return EntityPos.get_x();
}

int Entity::get_posY(void) {
	return EntityPos.get_y();
}

void Entity::setPos(int h, int j) {
	EntityPos.setPosition(h, j);
}

std::string Entity::get_face(void) {
	return face;
}

WORD Entity::get_colour(void) {
	return colour;
}

int Entity::get_patrol_dir(void) {
	return 0;
}