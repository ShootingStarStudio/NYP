#include "Guard.h"
Guard::Guard() {
	hp = 1;
	dmg = 2;
	def = 0;
	patrol_dir = rand() % 2; // 0-(U,D) 1-(L,R)
	face = "++";
    colour = 0xB0;
}
Guard::~Guard() {

}
void Guard::move(int dir) {
    if (patrol_dir == 0) {
        switch (dir) {
        case 0:
        case 1: // up
            EntityPos.setPosition(get_posX(), get_posY() - 1); break;
        case 2:
        case 3: // down
            EntityPos.setPosition(get_posX(), get_posY() + 1); break;
        }
    }
    if (patrol_dir == 1) {
        switch (dir) {
        case 0:
        case 1: // left
            EntityPos.setPosition(get_posX() - 2, get_posY()); break;
        case 2:
        case 3: // right
            EntityPos.setPosition(get_posX() + 2, get_posY()); break;
        }
    }
}

int Guard::get_patrol_dir(){
    return patrol_dir;
}