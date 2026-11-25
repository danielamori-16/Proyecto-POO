
#include "RobotAtaque.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

// ========================================
// CONSTRUCTOR DEL ROBOT DE ATAQUE
// ========================================

RobotAtaque::RobotAtaque(
    const Personaje& objetivo,
    sf::Vector2f posicionInicial
)
    : Robot(
        "Robot de Ataque",
        posicionInicial,
        100.f,
        10
      ),
      objetivo(objetivo),
      posicionInicial(posicionInicial) {

    setPosicion(posicionInicial);

    // Figura de respaldo.
    figura.setFillColor(
        sf::Color(200, 50, 65)
    );

    // ========================================
    // CARGAR SPRITESHEET
    // ========================================

    if (textura.loadFromFile(
        "assets/imagenes/RobotAtaque.png"
    )) {

        sf::Vector2u tamano = textura.getSize();

        std::cout
            << "[ROBOT ATAQUE] Textura cargada: "
            << tamano.x << " x " << tamano.y
            << std::endl;

        // Verificar dimensiones correctas.
        if (tamano.x == 384 &&
            tamano.y == 512) {

            textura.setSmooth(false);

            sprite.emplace(textura);

            sprite->setTextureRect(
                sf::IntRect(
                    {0, 0},
                    {128, 128}
                )
            );

            // Centrar el sprite.
            sprite->setOrigin({
                64.f,
                64.f
            });

            // Tamaño visual de 60 x 60 px.
            // Su colision sigue siendo 40 x 40.
            constexpr float tamanoVisual = 60.f;

            sprite->setScale({
                tamanoVisual / 128.f,
                tamanoVisual / 128.f
            });

            actualizarSprite();

        } else {

            std::cout
                << "[ROBOT ATAQUE] ERROR: "
                << "dimensiones incorrectas."
                << std::endl;
        }

    } else {

        std::cout
            << "[ROBOT ATAQUE] ERROR: "
            << "no se encontro RobotAtaque.png"
            << std::endl;
    }
}

// ========================================
// ACTUALIZAR SPRITE
// ========================================

void RobotAtaque::actualizarSprite() {

    if (!sprite.has_value()) {
        return;
    }

    int columna = std::clamp(
        cuadroActual, 0, 2
    );

    int fila = std::clamp(
        direccionVisual, 0, 3
    );

    // Seleccionar cuadro del spritesheet.
    sprite->setTextureRect(
        sf::IntRect(
            {
                columna * ANCHO_CUADRO,
                fila * ALTO_CUADRO
            },
            {
                ANCHO_CUADRO,
                ALTO_CUADRO
            }
        )
    );

    // Centrar la imagen sobre la
    // posicion logica de 40 x 40 px.
    sprite->setPosition({
        posicion.x + 20.f,
        posicion.y + 20.f
    });
}

// ========================================
// ORIENTAR HACIA EL CIENTIFICO
// ========================================

void RobotAtaque::orientarHaciaObjetivo() {

    sf::Vector2f diferencia =
        objetivo.getPosicion() - posicion;

    // Determinar hacia donde mirar.
    if (std::abs(diferencia.x) >
        std::abs(diferencia.y)) {

        if (diferencia.x > 0.f) {
            direccionVisual = 2; // Derecha
        } else {
            direccionVisual = 1; // Izquierda
        }

    } else {

        if (diferencia.y > 0.f) {
            direccionVisual = 0; // Abajo
        } else {
            direccionVisual = 3; // Arriba
        }
    }
}

// ========================================
// ACTUALIZAR ROBOT
// ========================================

void RobotAtaque::update(float dt) {

    // ========================================
    // ATURDIMIENTO
    // ========================================

    if (tiempoAturdido > 0.f) {

        tiempoAturdido = std::max(
            0.f,
            tiempoAturdido - dt
        );

        cuadroActual = 0;
        tiempoAnimacionDisparo = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // DETECTAR AL CIENTIFICO
    // ========================================

    sf::Vector2f destino =
        objetivo.getPosicion();

    float dx = destino.x - posicion.x;
    float dy = destino.y - posicion.y;

    float distancia = std::sqrt(
        dx * dx + dy * dy
    );

    // ========================================
    // ORIENTACION VISUAL
    // ========================================

    // El robot permanece fijo,
    // pero mira hacia el cientifico.
    orientarHaciaObjetivo();

    // ========================================
    // ANIMACION DE DISPARO
    // ========================================

    if (tiempoAnimacionDisparo > 0.f) {

        tiempoAnimacionDisparo = std::max(
            0.f,
            tiempoAnimacionDisparo - dt
        );

        // Mostrar temporalmente el cuadro 2.
        cuadroActual = 2;

    } else {

        // Posicion normal del robot.
        cuadroActual = 0;
    }

    // ========================================
    // COMPROBAR RANGO
    // ========================================

    if (distancia > rangoAtaque) {

        tiempoDisparo = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // TEMPORIZADOR DE DISPARO
    // ========================================

    tiempoDisparo += dt;

    if (tiempoDisparo >= intervaloDisparo) {

        atacar();

        tiempoDisparo = 0.f;

        // Activar una breve animacion.
        tiempoAnimacionDisparo = 0.2f;
        cuadroActual = 2;
    }

    actualizarSprite();
}

// ========================================
// DIBUJAR ROBOT DE ATAQUE
// ========================================

void RobotAtaque::draw(
    sf::RenderWindow& ventana
) {

    if (sprite.has_value()) {

        actualizarSprite();

        ventana.draw(*sprite);

    } else {

        // Figura roja de respaldo.
        ventana.draw(figura);
    }
}

// ========================================
// ATAQUE A DISTANCIA
// ========================================

void RobotAtaque::atacar() {

    // POO - POLIMORFISMO:
    // A diferencia del guardian,
    // este robot genera ataques a distancia.

    disparoPendiente = true;
}

// ========================================
// CONSULTAR DISPARO
// ========================================

bool RobotAtaque::consumirDisparo() {

    if (!disparoPendiente) {
        return false;
    }

    // Consumir el evento una sola vez.
    disparoPendiente = false;

    return true;
}

// ========================================
// ATURDIMIENTO ELECTRICO
// ========================================

void RobotAtaque::aturdir(
    float duracion
) {

    tiempoAturdido = std::max(
        0.f,
        duracion
    );

    // Cancelar disparos pendientes.
    disparoPendiente = false;

    tiempoAnimacionDisparo = 0.f;
    cuadroActual = 0;

    actualizarSprite();
}

bool RobotAtaque::estaAturdido() const {

    return tiempoAturdido > 0.f;
}

// ========================================
// REINICIAR ROBOT
// ========================================

void RobotAtaque::reiniciar() {

    // Restaurar vida y posicion.
    vida = 100;

    setPosicion(posicionInicial);

    // Reiniciar temporizadores.
    tiempoDisparo = 0.f;
    tiempoAturdido = 0.f;
    tiempoAnimacionDisparo = 0.f;

    disparoPendiente = false;

    // Restaurar animacion.
    cuadroActual = 0;
    direccionVisual = 0;

    figura.setFillColor(
        sf::Color(200, 50, 65)
    );

    actualizarSprite();
}
