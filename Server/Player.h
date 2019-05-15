#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(float x, float y, Dir dir_);


private:
    int health;
    bool live;
};
