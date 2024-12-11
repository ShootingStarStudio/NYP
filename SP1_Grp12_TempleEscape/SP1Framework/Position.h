#pragma once
class Position {
public:
	Position();
	~Position();
	void setPosition(int, int);

	int get_x(void);
	int get_y(void);
	

private:
	int x, y;
};