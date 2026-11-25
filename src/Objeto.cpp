
#include "Objeto.hpp"

#include <iostream>
#include <utility>

Objeto::Objeto(
    std::string nombre,
    int valor,
    sf::Vector2f posicion
)
    : nombre(std::move(nombre)),
      valor(valor),
      recogido(false),
      figura(10.f) {

    // POO - ENCAPSULAMIENTO:
    // La figura conserva la posicion y el area
    // utilizada para detectar recolecciones.

    figura.setFillColor(sf::Color::Yellow);
    figura.setPosition(posicion);

    // POO - COMPOSICION:
    // Objeto administra el recurso grafico
    // utilizado para representar la bateria.

    textura = std::make_shared<sf::Texture>();

    if (!textura->loadFromFile(
        "assets/imagenes/bateria_energia.png"
    )) {
        std::cerr
            << "No se pudo cargar bateria_energia.png\n";

        textura.reset();
        return;
    }

    sprite.emplace(*textura);

    sf::Vector2u tamanoImagen = textura->getSize();

    if (tamanoImagen.x > 0 && tamanoImagen.y > 0) {

        // La bateria se dibuja dentro de un area
        // de 28 x 36 pixeles.

        sprite->setScale({
            40.f / static_cast<float>(tamanoImagen.x),
            50.f / static_cast<float>(tamanoImagen.y)
        });

        sprite->setPosition({
            posicion.x - 10.f,
            posicion.y - 15.f
        });
    }
}

// POO - ENCAPSULAMIENTO:
// La recoleccion modifica un atributo privado.

void Objeto::recoger() {
    recogido = true;
}

bool Objeto::estaRecogido() const {
    return recogido;
}

int Objeto::getValor() const {
    return valor;
}

// POO - ABSTRACCION:
// Juego consulta los limites sin conocer
// como se representan internamente.

sf::FloatRect Objeto::getLimites() const {
    return figura.getGlobalBounds();
}

void Objeto::draw(sf::RenderWindow& ventana) const {

    if (recogido) {
        return;
    }

    if (sprite.has_value()) {
        ventana.draw(*sprite);
    } else {
        ventana.draw(figura);
    }
}
