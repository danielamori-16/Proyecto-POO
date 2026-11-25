
#include "Cientifico.hpp"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <array>

// POO - HERENCIA:
// Cientifico hereda los atributos y metodos de Personaje.

Cientifico::Cientifico()
    : Personaje("Cientifico", 100, {320.f, 280.f}, 250.f) {

    figura.setFillColor(sf::Color::Green);

    // POO - ENCAPSULAMIENTO:
    // Cientifico administra internamente su imagen.
    // Otras clases no necesitan cargarla.

    if (!textura.loadFromFile(
        "assets/imagenes/gato_cientifico_sheet.png"
    )) {
        std::cerr << "Error al cargar el sprite del gato\n";
        return;
    }

    // POO - COMPOSICION:
    // El sprite utiliza la textura que pertenece al Cientifico.

    sprite.emplace(textura);

    // Seleccionamos un dibujo de la hoja completa.
    // Por ahora utilizaremos la primera pose frontal.

    sprite->setTextureRect(
        sf::IntRect({340, 12}, {183, 242})
    );

    // Ajustamos la imagen al espacio del personaje.

    sprite->setScale({
        36.f / 183.f,
        40.f / 242.f
    });

    sprite->setPosition({
        posicion.x + 2.f,
        posicion.y
    });
}

// POO - POLIMORFISMO:
// Sobrescribimos update() de Personaje.

void Cientifico::update(float dt) {

    sf::Vector2f movimiento(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movimiento.y -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movimiento.y += 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movimiento.x -= 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movimiento.x += 1.f;

    float longitud = std::sqrt(
        movimiento.x * movimiento.x +
        movimiento.y * movimiento.y
    );

    if (longitud > 0.f) {
        movimiento /= longitud;
    }

    posicion += movimiento * velocidad * dt;

    posicion.x = std::clamp(posicion.x, 0.f, 1160.f);
    posicion.y = std::clamp(posicion.y, 0.f, 760.f);

    
figura.setPosition(posicion);

// POO - ENCAPSULAMIENTO:
// Cientifico controla internamente la animacion
// sin que Juego tenga que administrar los frames.

if (sprite.has_value()) {

    sprite->setPosition({
        posicion.x + 2.f,
        posicion.y
    });

    
    // POO - ENCAPSULAMIENTO:
    // Cientifico controla sus animaciones internamente.
    // Juego solo necesita llamar al metodo update().

    const std::array<std::array<sf::IntRect, 3>, 4> animaciones = {{
        {{
            sf::IntRect({340, 12}, {183, 242}),
            sf::IntRect({620, 12}, {183, 242}),
            sf::IntRect({900, 12}, {183, 242})
        }},
        {{
            sf::IntRect({340, 285}, {183, 242}),
            sf::IntRect({620, 285}, {183, 242}),
            sf::IntRect({900, 285}, {183, 242})
        }},
        {{
            sf::IntRect({340, 550}, {183, 242}),
            sf::IntRect({620, 550}, {183, 242}),
            sf::IntRect({900, 550}, {183, 242})
        }},
        {{
            sf::IntRect({340, 810}, {183, 242}),
            sf::IntRect({620, 810}, {183, 242}),
            sf::IntRect({900, 810}, {183, 242})
        }}
    }};

    // Determinar la direccion del movimiento.
    // 0 = abajo, 1 = izquierda, 2 = derecha, 3 = arriba.

    if (longitud > 0.f) {

        if (std::abs(movimiento.x) > std::abs(movimiento.y)) {
            direccion = movimiento.x < 0.f ? 1 : 2;
        } else {
            direccion = movimiento.y < 0.f ? 3 : 0;
        }

        tiempoAnimacion += dt;

        if (tiempoAnimacion >= 0.15f) {
            frameActual = (frameActual + 1) % 3;
            tiempoAnimacion = 0.f;
        }

    } else {

        frameActual = 0;
        tiempoAnimacion = 0.f;
    }

    sprite->setTextureRect(
        animaciones[direccion][frameActual]
    );

}

}

// POO - POLIMORFISMO:
// Cientifico utiliza su propia implementacion de draw().

void Cientifico::draw(sf::RenderWindow& ventana) {

    if (sprite.has_value()) {
        ventana.draw(*sprite);
    } else {
        ventana.draw(figura);
    }
}

void Cientifico::reiniciar() {

    // POO - HERENCIA:
    // Restablecemos los atributos protegidos
    // que Cientifico hereda de Personaje.

    vida = 100;
    posicion = {320.f, 280.f};

    figura.setPosition(posicion);

    // POO - ENCAPSULAMIENTO:
    // La animacion se reinicia internamente.
    tiempoAnimacion = 0.f;
    frameActual = 0;
    direccion = 0;

    if (sprite.has_value()) {

        sprite->setTextureRect(
            sf::IntRect({340, 12}, {183, 242})
        );

        sprite->setPosition({
            posicion.x + 2.f,
            posicion.y
        });
    }
}
int Cientifico::getDireccion() const {
    return direccion;
}