#pragma once

#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"


class KeyboardInterface
{
public:
    conf::Dir get_direction();
    bool get_shoot();
};

struct PlayerInput
{
    conf::Dir dir;
    bool is_shoot;
};
