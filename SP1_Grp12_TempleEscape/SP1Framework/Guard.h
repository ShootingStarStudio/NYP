#pragma once
#include "Entity.h"
class Guard :
    public Entity
{
public:
    Guard();
    ~Guard();
    void move(int);
    int get_patrol_dir(void);
private:
    int patrol_dir;
};

