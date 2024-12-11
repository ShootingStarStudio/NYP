#include "entity.h"

entity::entity(float X, float Y, float Z) {
    ECoords[0] = X; ECoords[1] = Y; ECoords[2] = Z;
}
entity::~entity() {

}
int entity::getstationtier(void) {
    return stationtier;
}
void entity::setstationtier(int newtier) {
    stationtier = newtier;
}

int entity::getworkertier(void) {
    return workertier;
}
void entity::setworkertier(int newtier) {
    workertier = newtier;
}

float entity::getprofit(void) {
    profit = (stationtier * 5) * (workertier * 5);
    return profit;
}
int entity::getWage()
{
    if (workertier == 1)
        return Tier1Wage;
    else if (workertier == 2)
        return Tier2Wage;
    else if (workertier == 3)
        return Tier3Wage;
    else
        return 0;
}
//
//void entity::setstationcost(float newcost) {
//    stationcost = newcost;
//}
float entity::getstationcost(void) {
    if (stationtier == 0)
        return StationUpgrade0;
    else if (stationtier == 1)
        return StationUpgrade1;
    else if (stationtier == 2)
        return StationUpgrade2;
    else
        return 0;
}/*
void entity::setworkercost(float newcost) {
    workercost = newcost;
}*/
float entity::getworkercost() {
    if (workertier == 0)
        return WorkerUpgrade0;
    else if (workertier == 1)
        return WorkerUpgrade1;
    else if (workertier == 2)
        return Workerupgrade2;
    else
        return 0;
}