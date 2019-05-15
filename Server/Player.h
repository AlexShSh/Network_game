#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(float x, float y, conf::Dir dir_);

private:
    int health;
    bool live;
};
