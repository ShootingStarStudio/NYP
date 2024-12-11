#include "Position.h"

Position::Position() {
	x = 0, y = 0;
}
Position::~Position() {

}
void Position::setPosition(int xNew, int yNew) {
	x = xNew;
	y = yNew;
}

int Position::get_x(void) {
	return x;
}


int Position::get_y(void) {
	return y;
}
