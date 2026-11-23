
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include <memory>

class Objeto {

private:

    // POO - ENCAPSULAMIENTO:
    // Los atributos son privados para evitar
    // modificaciones directas desde otras clases.
    std::string nombre;
    int valor;
    bool recogido;

    // Conservamos la figura para detectar colisiones.
    sf::CircleShape figura;

    // POO - COMPOSICION:
    // La textura y el sprite forman parte de Objeto.
    // Sus ciclos de vida estan vinculados al objeto.
    std::shared_ptr<sf::Texture> textura;
    std::optional<sf::Sprite> sprite;

public:

    Objeto(
        std::string nombre,
        int valor,
        sf::Vector2f posicion
    );

    // POO - ENCAPSULAMIENTO:
    // Los metodos publicos controlan el acceso
    // al estado del objeto.
    void recoger();
    bool estaRecogido() const;
    int getValor() const;

    sf::FloatRect getLimites() const;
    void draw(sf::RenderWindow& ventana) const;
};
