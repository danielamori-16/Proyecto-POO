
#pragma once

#include "Personaje.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

// POO - HERENCIA:
// Cientifico hereda los atributos y metodos
// generales de la clase Personaje.

class Cientifico : public Personaje {

private:

    // POO - ENCAPSULAMIENTO:
    // La textura y el sprite son privados.
    // Otras clases no pueden modificarlos directamente.

    sf::Texture textura;
    std::optional<sf::Sprite> sprite;

    // POO - COMPOSICION:
    // La textura y el sprite forman parte del Cientifico.
    // Sus ciclos de vida dependen de este objeto.

    // Variables para controlar las animaciones.
    float tiempoAnimacion = 0.f;
    int frameActual = 0;
    int direccion = 0;

public:

    Cientifico();
    // POO - ENCAPSULAMIENTO:
    // Juego puede consultar la direccion sin modificarla.
    int getDireccion() const;
    // POO - POLIMORFISMO:
    // Sobrescribimos los metodos virtuales
    // heredados de Personaje.

    void update(float dt) override;

    void draw(sf::RenderWindow& ventana) override;
    // POO - ENCAPSULAMIENTO:
    // El cientifico restaura su propio estado.
    void reiniciar();
};
