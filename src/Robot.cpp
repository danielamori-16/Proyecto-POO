
#include "Robot.hpp"

Robot::Robot(
    std::string nombre,
    sf::Vector2f posicion,
    float velocidad,
    int danio
) : Personaje(nombre, 100, posicion, velocidad),
    danio(danio) {

    figura.setFillColor(sf::Color::Red);
}
