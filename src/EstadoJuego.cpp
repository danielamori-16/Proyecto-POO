#include "EstadoJuego.hpp"
#include "Juego.hpp"
#include <utility>

// PATRON DE DISENO: STATE

// ESTADO MENU

void EstadoMenu::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosMenu(evento);
}

void EstadoMenu::actualizar(Juego& juego, float dt) {}

void EstadoMenu::dibujar(Juego& juego) {
    juego.dibujarMenu();
}

// ESTADO JUGANDO

void EstadoJugando::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosJugando(evento);
}

void EstadoJugando::actualizar(Juego& juego, float dt) {
    juego.actualizarPartida(dt);
}

void EstadoJugando::dibujar(Juego& juego) {
    juego.dibujarPartida();
}

// ESTADO PAUSA

void EstadoPausa::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosPausa(evento);
}

void EstadoPausa::actualizar(Juego& juego, float dt) {
    // La partida permanece congelada.
}

void EstadoPausa::dibujar(Juego& juego) {
    juego.dibujarPausa();
}

// ESTADO GAME OVER

void EstadoGameOver::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosResultado(evento, false);
}

void EstadoGameOver::actualizar(Juego& juego, float dt) {}

void EstadoGameOver::dibujar(Juego& juego) {
    juego.dibujarResultado(false);
}

// ESTADO VICTORIA

void EstadoVictoria::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosResultado(evento, true);
}

void EstadoVictoria::actualizar(Juego& juego, float dt) {}

void EstadoVictoria::dibujar(Juego& juego) {
    juego.dibujarResultado(true);
}

// ESTADO CONTROLES

void EstadoControles::procesarEventos(Juego& juego, sf::Event& evento) {
    juego.eventosControles(evento);
}

void EstadoControles::actualizar(Juego& juego, float dt) {}

void EstadoControles::dibujar(Juego& juego) {
    juego.dibujarControles();
}

// TRANSICIONES DEL PATRON STATE

void Juego::cambiarEstado(std::unique_ptr<EstadoJuego> nuevoEstado) {
    estadoPendiente = std::move(nuevoEstado);
}

void Juego::aplicarCambioEstado() {
    if (estadoPendiente) {
        estadoActual = std::move(estadoPendiente);
    }
}

void Juego::irAMenu() {
    cambiarEstado(std::make_unique<EstadoMenu>());
}

void Juego::irAJugando() {
    cambiarEstado(std::make_unique<EstadoJugando>());
}

void Juego::irAPausa() {
    cambiarEstado(std::make_unique<EstadoPausa>());
}

void Juego::irAGameOver() {
    cambiarEstado(std::make_unique<EstadoGameOver>());
}

void Juego::irAVictoria() {
    cambiarEstado(std::make_unique<EstadoVictoria>());
}

void Juego::irAControles() {
    cambiarEstado(std::make_unique<EstadoControles>());
}