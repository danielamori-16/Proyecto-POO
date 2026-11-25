
#include "Personaje.hpp"
#include <algorithm>

Personaje::Personaje(
    std::string nombre,
    int vida,
    sf::Vector2f posicion,
    float velocidad
) : nombre(nombre),
    vida(vida),
    posicion(posicion),
    velocidad(velocidad),
    figura({40.f, 40.f}) {

    figura.setPosition(posicion);
}

void Personaje::recibirDanio(int cantidad) {
    vida = std::max(0, vida - std::max(0, cantidad));
}

int Personaje::getVida() const {
    return vida;
}

sf::Vector2f Personaje::getPosicion() const {
    return posicion;
}

sf::FloatRect Personaje::getLimites() const {
    return figura.getGlobalBounds();
}
void Personaje::setPosicion(sf::Vector2f nuevaPosicion) {
    posicion = nuevaPosicion;
    figura.setPosition(posicion);
}