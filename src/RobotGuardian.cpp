
#include "RobotGuardian.hpp"
#include "Nivel.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

// ========================================
// CONSTRUCTOR DEL ROBOT GUARDIAN
// ========================================

RobotGuardian::RobotGuardian(
    sf::Vector2f inicio,
    sf::Vector2f final
)
    : Robot("Robot Guardian", inicio, 100.f, 10),
      puntoInicial(inicio),
      puntoFinal(final) {

    // Posicion inicial del robot.
    setPosicion(puntoInicial);

    // Figura de respaldo si falla el PNG.
    figura.setFillColor(
        sf::Color(230, 160, 50)
    );

    // ========================================
    // CARGAR SPRITESHEET
    // ========================================

    if (textura.loadFromFile(
        "assets/imagenes/RobotGuardian.png"
    )) {

        sf::Vector2u tamano = textura.getSize();

        std::cout
            << "[GUARDIAN] Textura cargada: "
            << tamano.x << " x " << tamano.y
            << std::endl;

        // Esperamos 3 columnas x 4 filas,
        // con cuadros de 128 x 128 px.
        if (tamano.x == 384 && tamano.y == 512) {

            textura.setSmooth(false);

            // SFML 3: crear sprite con textura.
            sprite.emplace(textura);

            sprite->setTextureRect(
                sf::IntRect(
                    {0, 0},
                    {128, 128}
                )
            );

            // Centrar el origen del cuadro.
            sprite->setOrigin({64.f, 64.f});

            // Tamano visual: 60 x 60 px.
            // Las colisiones siguen siendo 40 x 40.
            constexpr float tamanoVisual = 60.f;

            sprite->setScale({
                tamanoVisual / 128.f,
                tamanoVisual / 128.f
            });

            actualizarSprite();

        } else {

            std::cout
                << "[GUARDIAN] ERROR: dimensiones "
                << "inesperadas del spritesheet."
                << std::endl;
        }

    } else {

        std::cout
            << "[GUARDIAN] ERROR: no se encontro "
            << "RobotGuardian.png"
            << std::endl;
    }
}

// ========================================
// ASOCIAR EL MAPA AL GUARDIAN
// ========================================

void RobotGuardian::establecerNivel(
    const Nivel& nuevoNivel
) {

    // POO - ASOCIACION:
    // El robot consulta el mapa existente,
    // pero no es propietario de el.
    nivel = &nuevoNivel;
}

// ========================================
// ACTUALIZAR SPRITE
// ========================================

void RobotGuardian::actualizarSprite() {

    if (!sprite.has_value()) {
        return;
    }

    // Limitar los indices para evitar
    // seleccionar cuadros incorrectos.
    int columna = std::clamp(cuadroActual, 0, 2);
    int fila = std::clamp(direccionVisual, 0, 3);

    // Seleccionar cuadro del spritesheet.
    sprite->setTextureRect(
        sf::IntRect(
            {
                columna * 128,
                fila * 128
            },
            {
                128,
                128
            }
        )
    );

    // Centrar sprite de 60 x 60 px
    // sobre la colision de 40 x 40 px.
    sprite->setPosition({
        posicion.x + 20.f,
        posicion.y + 20.f
    });
}

// ========================================
// ACTUALIZAR MOVIMIENTO
// ========================================

void RobotGuardian::update(float dt) {

    // ========================================
    // ATURDIMIENTO
    // ========================================

    if (tiempoAturdido > 0.f) {

        tiempoAturdido = std::max(
            0.f,
            tiempoAturdido - dt
        );

        cuadroActual = 0;
        tiempoAnimacion = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // ESPERA EN EXTREMOS
    // ========================================

    if (tiempoEspera > 0.f) {

        tiempoEspera = std::max(
            0.f,
            tiempoEspera - dt
        );

        cuadroActual = 0;
        tiempoAnimacion = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // SELECCIONAR DESTINO
    // ========================================

    sf::Vector2f destino =
        haciaFinal ? puntoFinal : puntoInicial;

    sf::Vector2f diferencia =
        destino - posicion;

    float distancia = std::sqrt(
        diferencia.x * diferencia.x +
        diferencia.y * diferencia.y
    );

    // Llegada al extremo de patrullaje.
    if (distancia <= 2.f) {

        setPosicion(destino);

        haciaFinal = !haciaFinal;
        tiempoEspera = 0.1f;

        cuadroActual = 0;
        tiempoAnimacion = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // DIRECCION DEL SPRITE
    // ========================================

    // 0 = abajo
    // 1 = izquierda
    // 2 = derecha
    // 3 = arriba

    if (std::abs(diferencia.x) >
        std::abs(diferencia.y)) {

        direccionVisual =
            diferencia.x > 0.f ? 2 : 1;

    } else {

        direccionVisual =
            diferencia.y > 0.f ? 0 : 3;
    }

    // ========================================
    // CALCULAR MOVIMIENTO
    // ========================================

    sf::Vector2f direccionMovimiento =
        diferencia / distancia;

    float paso = std::min(
        velocidad * dt,
        distancia
    );

    sf::Vector2f posicionAnterior = posicion;

    sf::Vector2f nuevaPosicion =
        posicion + direccionMovimiento * paso;

    setPosicion(nuevaPosicion);

    // ========================================
    // COLISIONES CON PAREDES
    // ========================================

    // POO - ASOCIACION:
    // Consultamos Nivel antes de aceptar
    // el movimiento del robot.
    if (nivel != nullptr &&
        nivel->hayColision(getLimites())) {

        // Cancelar movimiento.
        setPosicion(posicionAnterior);

        // Cambiar sentido del patrullaje.
        haciaFinal = !haciaFinal;

        // Pequeña pausa.
        tiempoEspera = 0.5f;

        cuadroActual = 0;
        tiempoAnimacion = 0.f;

        actualizarSprite();
        return;
    }

    // ========================================
    // ANIMACION DE CAMINATA
    // ========================================

    tiempoAnimacion += dt;

    if (tiempoAnimacion >= intervaloAnimacion) {

        tiempoAnimacion = 0.f;

        cuadroActual = (cuadroActual + 1) % 3;
    }

    actualizarSprite();
}

// ========================================
// DIBUJAR ROBOT
// ========================================

void RobotGuardian::draw(
    sf::RenderWindow& ventana
) {

    if (sprite.has_value()) {

        actualizarSprite();
        ventana.draw(*sprite);

    } else {

        // Mostrar figura naranja si
        // no se encuentra la textura.
        ventana.draw(figura);
    }
}

// ========================================
// ATAQUE DEL GUARDIAN
// ========================================

void RobotGuardian::atacar() {

    // Juego administra el daño por contacto.
}

// ========================================
// DIFICULTAD SEGUN NIVEL
// ========================================

void RobotGuardian::establecerDificultad(
    int numeroNivel
) {

    if (numeroNivel == 1) {
        velocidad = 40.f;
    }
    else if (numeroNivel == 2) {
        velocidad = 60.f;
    }
    else {
        velocidad = 80.f;
    }
}

// ========================================
// ATURDIMIENTO ELECTRICO
// ========================================

void RobotGuardian::aturdir(float duracion) {

    tiempoAturdido = std::max(
        0.f,
        duracion
    );

    cuadroActual = 0;
    tiempoAnimacion = 0.f;

    actualizarSprite();
}

bool RobotGuardian::estaAturdido() const {

    return tiempoAturdido > 0.f;
}

// ========================================
// REINICIAR GUARDIAN
// ========================================

void RobotGuardian::reiniciar() {

    vida = 100;

    setPosicion(puntoInicial);

    haciaFinal = true;

    tiempoEspera = 0.f;
    tiempoAturdido = 0.f;

    cuadroActual = 0;
    tiempoAnimacion = 0.f;
    direccionVisual = 2;

    figura.setFillColor(
        sf::Color(230, 160, 50)
    );

    actualizarSprite();
}
