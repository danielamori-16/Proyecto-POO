
#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Personaje {
protected:
    std::string nombre;
    int vida;
    sf::Vector2f posicion;
    float velocidad;

    sf::RectangleShape figura;

public:
    Personaje(
        std::string nombre,
        int vida,
        sf::Vector2f posicion,
        float velocidad
    );

    virtual ~Personaje() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& ventana) = 0;

    void recibirDanio(int cantidad);
    int getVida() const;
    sf::Vector2f getPosicion() const;

    sf::FloatRect getLimites() const;

    void setPosicion(sf::Vector2f nuevaPosicion);
};
