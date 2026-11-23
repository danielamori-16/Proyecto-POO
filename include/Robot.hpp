
#pragma once

#include "Personaje.hpp"

class Robot : public Personaje {
protected:
    int danio;

public:
    Robot(
        std::string nombre,
        sf::Vector2f posicion,
        float velocidad,
        int danio
    );

    virtual void atacar() = 0;
};
