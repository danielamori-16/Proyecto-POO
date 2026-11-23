
#include "Puntuacion.hpp"

Puntuacion::Puntuacion() : puntos(0) {
}

void Puntuacion::agregarPuntos(int cantidad) {
    if (cantidad > 0) {
        puntos += cantidad;
    }
}

int Puntuacion::getPuntos() const {
    return puntos;
}

void Puntuacion::reiniciar() {
    puntos = 0;
}
