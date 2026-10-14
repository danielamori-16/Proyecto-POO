
#ifndef PERSONAJE_HPP
#define PERSONAJE_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Personaje {
protected:
    std::string nombre;
    int vida;
    float velocidad;
    sf::Vector2f posicion;

public:
    Personaje(std::string nombre, int vida,
              float velocidad, sf::Vector2f posicion);

    virtual ~Personaje() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& ventana) = 0;

    void recibirDanio(int cantidad);
    int getVida() const;
};

#endif
