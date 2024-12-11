#include "Golem.h"

Golem::Golem() {
	hp = 15;
	dmg = 3;
	def = 1;
    face = "==";
    colour = 0x80;
}

Golem::~Golem()
{

}

void Golem::move(int dir) {
    switch (dir) {
    case 0: // up
        EntityPos.setPosition(get_posX(), get_posY() - 1); break;
    case 1: // down
        EntityPos.setPosition(get_posX(), get_posY() + 1); break;
    case 2: // left
        EntityPos.setPosition(get_posX() - 2, get_posY()); break;
    case 3:
        EntityPos.setPosition(get_posX() + 2, get_posY()); break;
    }
}