
#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <optional>
#include <iostream>

// ========================================
// ARMA ELECTRICA DEL CIENTIFICO
// ========================================

class ArmaElectrica {

private:

    // POO - ENCAPSULAMIENTO:
    // El arma administra sus recursos
    // y su efecto de disparo.

    sf::Texture textura;
    std::optional<sf::Sprite> sprite;

    // Figuras de respaldo.
    sf::RectangleShape cuerpo;
    sf::RectangleShape canon;
    sf::RectangleShape empunadura;

    sf::Vector2f posicionJugador{0.f, 0.f};

    // 0 = abajo
    // 1 = izquierda
    // 2 = derecha
    // 3 = arriba
    int direccionActual = 2;

    float tiempoEfecto = 0.f;
    const float duracionEfecto = 0.15f;

    static constexpr int ANCHO_CUADRO = 128;
    static constexpr int ALTO_CUADRO = 128;

    // ========================================
    // ACTUALIZAR SPRITE
    // ========================================

    void actualizarSprite() {

        if (!sprite.has_value()) {
            return;
        }

        // Orden de columnas del PNG:
        // 0 = abajo, 1 = izquierda,
        // 2 = derecha, 3 = arriba.
        int columna = std::clamp(
            direccionActual, 0, 3
        );

        // Fila 0: arma normal.
        // Fila 1: arma disparando.
        int fila = tiempoEfecto > 0.f ? 1 : 0;

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

        sf::Vector2f centro{
            posicionJugador.x + 20.f,
            posicionJugador.y + 20.f
        };

        // Colocar la pistola al costado
        // del cientifico segun su direccion.
        sf::Vector2f desplazamiento{0.f, 0.f};

        if (direccionActual == 0) {
            desplazamiento = {12.f, 17.f};
        }
        else if (direccionActual == 1) {
            desplazamiento = {-18.f, 8.f};
        }
        else if (direccionActual == 2) {
            desplazamiento = {18.f, 8.f};
        }
        else {
            desplazamiento = {12.f, -17.f};
        }

        sprite->setPosition(
            centro + desplazamiento
        );
    }

public:

    // ========================================
    // CONSTRUCTOR
    // ========================================

    ArmaElectrica() {

        // Colores de respaldo.
        cuerpo.setFillColor(
            sf::Color(45, 95, 180)
        );

        cuerpo.setOutlineThickness(1.f);
        cuerpo.setOutlineColor(
            sf::Color(130, 220, 255)
        );

        canon.setFillColor(sf::Color::Cyan);

        empunadura.setFillColor(
            sf::Color(35, 45, 70)
        );

        // ====================================
        // CARGAR SPRITESHEET
        // ====================================

        if (textura.loadFromFile(
            "assets/imagenes/PistolaElectrica.png"
        )) {

            sf::Vector2u tamano =
                textura.getSize();

            std::cout
                << "[ARMA] Textura cargada: "
                << tamano.x << " x "
                << tamano.y << std::endl;

            if (tamano.x == 512 &&
                tamano.y == 256) {

                textura.setSmooth(false);

                sprite.emplace(textura);

                sprite->setTextureRect(
                    sf::IntRect(
                        {0, 0},
                        {128, 128}
                    )
                );

                sprite->setOrigin({
                    64.f,
                    64.f
                });

                // Tamaño visual de 58 x 58.
                constexpr float tamanoVisual = 58.f;

                sprite->setScale({
                    tamanoVisual / 128.f,
                    tamanoVisual / 128.f
                });

                actualizarSprite();

            } else {

                std::cout
                    << "[ARMA] ERROR: dimensiones "
                    << "incorrectas."
                    << std::endl;
            }

        } else {

            std::cout
                << "[ARMA] ERROR: no se encontro "
                << "PistolaElectrica.png"
                << std::endl;
        }
    }

    // ========================================
    // ACTUALIZAR POSICION Y DIRECCION
    // ========================================

    void actualizar(
        sf::Vector2f posicion,
        int direccion
    ) {

        posicionJugador = posicion;
        direccionActual = direccion;

        actualizarSprite();

        // Figuras de respaldo.
        float x = posicion.x + 20.f;
        float y = posicion.y + 20.f;

        if (direccion == 2) {

            // Derecha.
            cuerpo.setSize({24.f, 12.f});
            cuerpo.setPosition({x + 5.f, y - 6.f});

            canon.setSize({14.f, 6.f});
            canon.setPosition({x + 29.f, y - 3.f});

            empunadura.setSize({7.f, 12.f});
            empunadura.setPosition({x + 10.f, y + 6.f});

        }
        else if (direccion == 1) {

            // Izquierda.
            cuerpo.setSize({24.f, 12.f});
            cuerpo.setPosition({x - 29.f, y - 6.f});

            canon.setSize({14.f, 6.f});
            canon.setPosition({x - 43.f, y - 3.f});

            empunadura.setSize({7.f, 12.f});
            empunadura.setPosition({x - 17.f, y + 6.f});

        }
        else if (direccion == 3) {

            // Arriba.
            cuerpo.setSize({12.f, 24.f});
            cuerpo.setPosition({x - 6.f, y - 29.f});

            canon.setSize({6.f, 14.f});
            canon.setPosition({x - 3.f, y - 43.f});

            empunadura.setSize({12.f, 7.f});
            empunadura.setPosition({x + 6.f, y - 17.f});

        }
        else {

            // Abajo.
            cuerpo.setSize({12.f, 24.f});
            cuerpo.setPosition({x - 6.f, y + 5.f});

            canon.setSize({6.f, 14.f});
            canon.setPosition({x - 3.f, y + 29.f});

            empunadura.setSize({12.f, 7.f});
            empunadura.setPosition({x + 6.f, y + 10.f});
        }
    }

    // ========================================
    // ACTIVAR DISPARO
    // ========================================

    void activarDisparo() {

        tiempoEfecto = duracionEfecto;

        actualizarSprite();
    }

    // ========================================
    // ACTUALIZAR EFECTO
    // ========================================

    void actualizarEfecto(float dt) {

        tiempoEfecto = std::max(
            0.f,
            tiempoEfecto - dt
        );

        actualizarSprite();
    }

    // ========================================
    // REINICIAR ARMA
    // ========================================

    void reiniciar() {

        tiempoEfecto = 0.f;

        actualizarSprite();
    }

    // ========================================
    // DIBUJAR ARMA
    // ========================================

    void dibujar(sf::RenderWindow& ventana) {

        if (sprite.has_value()) {

            // Diseño personalizado.
            ventana.draw(*sprite);

        } else {

            // Diseño anterior de respaldo.
            ventana.draw(empunadura);
            ventana.draw(cuerpo);
            ventana.draw(canon);
        }

        // ====================================
        // RAYO ELECTRICO
        // ====================================

        if (tiempoEfecto <= 0.f) {
            return;
        }

        // Determinar direccion.
        sf::Vector2f dir{0.f, 1.f};

        if (direccionActual == 1) {
            dir = {-1.f, 0.f};
        }
        else if (direccionActual == 2) {
            dir = {1.f, 0.f};
        }
        else if (direccionActual == 3) {
            dir = {0.f, -1.f};
        }

        sf::Vector2f perpendicular{
            -dir.y,
            dir.x
        };

        sf::Vector2f centro{
            posicionJugador.x + 20.f,
            posicionJugador.y + 20.f
        };

        // Inicio del rayo.
        sf::Vector2f inicio =
            centro + dir * 43.f;

        sf::VertexArray rayo(
            sf::PrimitiveType::LineStrip,
            7
        );

        const float desviaciones[7] = {
            0.f, -5.f, 7.f, -6.f,
            6.f, -4.f, 0.f
        };

        for (int i = 0; i < 7; i++) {

            float distancia = i * 23.f;

            rayo[i].position =
                inicio +
                dir * distancia +
                perpendicular * desviaciones[i];

            rayo[i].color =
                sf::Color(90, 235, 255);
        }

        ventana.draw(rayo);

        // Destello inicial.
        sf::CircleShape destello(5.f);

        destello.setOrigin({5.f, 5.f});
        destello.setPosition(inicio);

        destello.setFillColor(
            sf::Color::White
        );

        ventana.draw(destello);
    }
};
