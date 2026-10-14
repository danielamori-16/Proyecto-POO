
#include "Cientifico.hpp"

Cientifico::Cientifico(std::string nombre, int vida,
                       float velocidad, sf::Vector2f posicion)
    : Personaje(nombre, vida, velocidad, posicion),
      puntos(0) {

    // Representacion visual provisional del cientifico
    forma.setSize(sf::Vector2f(40.f, 50.f));
    forma.setFillColor(sf::Color(80, 200, 255));
    forma.setPosition(posicion);
}

void Cientifico::update(float dt) {
    mover(dt);
}

void Cientifico::mover(float dt) {
    float desplazamiento = velocidad * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        posicion.y -= desplazamiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        posicion.y += desplazamiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        posicion.x -= desplazamiento;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        posicion.x += desplazamiento;
    }

    // Evitar que el cientifico salga de la ventana
    if (posicion.x < 0.f) {
        posicion.x = 0.f;
    }

    if (posicion.y < 0.f) {
        posicion.y = 0.f;
    }

    if (posicion.x > 760.f) {
        posicion.x = 760.f;
    }

    if (posicion.y > 550.f) {
        posicion.y = 550.f;
    }

    forma.setPosition(posicion);
}

void Cientifico::draw(sf::RenderWindow& ventana) {
    ventana.draw(forma);
}

void Cientifico::recoger(Objeto* objeto) {
    if (objeto != nullptr) {
        inventario.push_back(objeto);
    }
}

void Cientifico::usarObjeto() {
    // Pendiente: implementar cuando se defina la interfaz
    // de la clase Objeto junto con el equipo.
}

int Cientifico::getPuntos() const {
    return puntos;
}
