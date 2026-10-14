
#ifndef CIENTIFICO_HPP
#define CIENTIFICO_HPP

#include "Personaje.hpp"
#include <vector>

class Objeto;

class Cientifico : public Personaje {
private:
    sf::RectangleShape forma;
    std::vector<Objeto*> inventario;
    int puntos;

public:
    Cientifico(std::string nombre, int vida,
               float velocidad, sf::Vector2f posicion);

    void update(float dt) override;
    void draw(sf::RenderWindow& ventana) override;

    void mover(float dt);
    void recoger(Objeto* objeto);
    void usarObjeto();

    int getPuntos() const;
};

#endif
