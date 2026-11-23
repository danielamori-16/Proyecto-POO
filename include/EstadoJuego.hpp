#pragma once
#include <SFML/Graphics.hpp>

class Juego;

// PATRON DE DISENO: STATE
// Clase abstracta para los estados del juego.

class EstadoJuego {
public:
    virtual ~EstadoJuego() = default;

    virtual void procesarEventos(
        Juego& juego, sf::Event& evento
    ) = 0;

    virtual void actualizar(
        Juego& juego, float dt
    ) = 0;

    virtual void dibujar(Juego& juego) = 0;
};

// ESTADOS CONCRETOS

class EstadoMenu : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};

class EstadoJugando : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};

class EstadoPausa : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};

class EstadoGameOver : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};

class EstadoVictoria : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};

class EstadoControles : public EstadoJuego {
public:
    void procesarEventos(Juego& juego, sf::Event& evento) override;
    void actualizar(Juego& juego, float dt) override;
    void dibujar(Juego& juego) override;
};