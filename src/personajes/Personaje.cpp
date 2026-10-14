#include "Personaje.hpp"

Personaje::Personaje(std::string nombre, int vida,
                     float velocidad, sf::Vector2f posicion) {
    this->nombre = nombre;
    this->vida = vida;
    this->velocidad = velocidad;
    this->posicion = posicion;
}

void Personaje::recibirDanio(int cantidad) {
    if (cantidad > 0) {
        vida -= cantidad;

        if (vida < 0) {
            vida = 0;
        }
    }
}

int Personaje::getVida() const {
    return vida;
}
