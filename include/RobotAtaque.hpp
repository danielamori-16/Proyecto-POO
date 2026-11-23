
#pragma once

#include "Robot.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

// POO - HERENCIA:
// RobotAtaque hereda de Robot.
class RobotAtaque : public Robot {

private:

    // ========================================
    // OBJETIVO DEL ROBOT
    // ========================================

    // POO - ASOCIACION:
    // Consulta al cientifico sin ser su propietario.
    const Personaje& objetivo;

    // POO - ENCAPSULAMIENTO:
    // Estado interno del robot.
    sf::Vector2f posicionInicial;

    float rangoAtaque = 300.f;
    float intervaloDisparo = 1.1f;

    float tiempoDisparo = 0.f;
    float tiempoAturdido = 0.f;

    bool disparoPendiente = false;

    // ========================================
    // SPRITE Y ANIMACION
    // ========================================

    // POO - COMPOSICION:
    // El robot administra su textura.
    sf::Texture textura;

    // SFML 3 requiere una textura
    // para construir un sprite.
    std::optional<sf::Sprite> sprite;

    // Spritesheet de 384 x 512.
    // 3 columnas y 4 filas.
    static constexpr int ANCHO_CUADRO = 128;
    static constexpr int ALTO_CUADRO = 128;

    int cuadroActual = 0;

    // Direcciones:
    // 0 = abajo
    // 1 = izquierda
    // 2 = derecha
    // 3 = arriba
    int direccionVisual = 0;

    // Efecto visual al disparar.
    float tiempoAnimacionDisparo = 0.f;

    // Actualizar cuadro y posicion.
    void actualizarSprite();

    // Orientar el robot hacia el cientifico.
    void orientarHaciaObjetivo();

public:

    RobotAtaque(
        const Personaje& objetivo,
        sf::Vector2f posicionInicial
    );

    // POO - POLIMORFISMO.
    void update(float dt) override;
    void draw(sf::RenderWindow& ventana) override;
    void atacar() override;

    bool consumirDisparo();

    void aturdir(float duracion);
    bool estaAturdido() const;

    void reiniciar();
};
