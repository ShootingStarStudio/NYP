#pragma once
#include "Entity.h"
class Hunter :
    public Entity
{
public:
    Hunter();
    ~Hunter();
    void move(int);
};

