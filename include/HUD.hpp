#pragma once

#include <SFML/Graphics.hpp>
#include "Observador.hpp"

// POO - SRP / ALTA COHESION:
// HUD solo representa en pantalla informacion de la partida.
// No controla reglas del juego ni modifica personajes.
class HUD : public Observador {
public:
    void dibujar(sf::RenderWindow& ventana, const sf::Font* fuente,
                 int vida, int energia, int nivel, int cargas) const;
    void actualizarEstado(
    int vida,
    int puntos,
    int nivel,
    int cargas
) override;
private:
    // POO - ENCAPSULAMIENTO:
    // El HUD almacena el estado recibido.
    int vidaActual = 100;
    int puntosActuales = 0;
    int nivelActual = 1;
    int cargasActuales = 5;
};
