#pragma once
#include <iostream>

#include "Entity.h"

using namespace std;

namespace fg
{
    class Truck : public Entity
    {
    public:
        Truck(float posX, float posY);
        void update(sf::Vector2u size);
    };
}