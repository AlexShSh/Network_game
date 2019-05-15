#pragma once

#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"


class KeyboardInterface
{
public:
    Dir get_direction();

private:
    Dir direction;
};
