#pragma once

#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"


class KeyboardInterface
{
public:
    conf::Dir get_direction();
    bool get_shoot();
};
