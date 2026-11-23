
#pragma once

#include "Robot.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

// Declaracion adelantada.
// Evita incluir Nivel.hpp innecesariamente aqui.
class Nivel;

// POO - HERENCIA:
// RobotGuardian hereda de Robot.
class RobotGuardian : public Robot {

private:
    // ========================================
    // PATRULLAJE
    // ========================================

    sf::Vector2f puntoInicial;
    sf::Vector2f puntoFinal;

    bool haciaFinal = true;

    float tiempoEspera = 0.f;
    float tiempoAturdido = 0.f;

    // POO - ASOCIACION:
    // El guardian consulta el mapa,
    // pero no es propietario del Nivel.
    const Nivel* nivel = nullptr;

    // ========================================
    // SPRITE Y ANIMACION
    // ========================================

    sf::Texture textura;
    std::optional<sf::Sprite> sprite;

    static constexpr int ANCHO_CUADRO = 128;
    static constexpr int ALTO_CUADRO = 128;

    int cuadroActual = 0;

    // 0 = abajo
    // 1 = izquierda
    // 2 = derecha
    // 3 = arriba
    int direccionVisual = 2;

    float tiempoAnimacion = 0.f;
    float intervaloAnimacion = 0.15f;

    void actualizarSprite();

public:
    RobotGuardian(
        sf::Vector2f inicio,
        sf::Vector2f final
    );

    void update(float dt) override;
    void draw(sf::RenderWindow& ventana) override;
    void atacar() override;

    // Asociar el mapa al guardian.
    void establecerNivel(const Nivel& nuevoNivel);

    void establecerDificultad(int numeroNivel);

    void aturdir(float duracion);
    bool estaAturdido() const;

    void reiniciar();
};
