
#pragma once

#include <vector>
#include <algorithm>

#include "Observador.hpp"

// ========================================
// PATRON OBSERVER - SUJETO
// ========================================

class NotificadorEstado {

private:

    // POO - AGREGACION:
    // El notificador guarda referencias
    // a observadores que no le pertenecen.
    std::vector<Observador*> observadores;

public:

    // Registrar un observador.
    void agregarObservador(Observador* observador) {

        if (observador == nullptr) {
            return;
        }

        // Evitar registrar el mismo
        // observador dos veces.
        auto encontrado = std::find(
            observadores.begin(),
            observadores.end(),
            observador
        );

        if (encontrado == observadores.end()) {
            observadores.push_back(observador);
        }
    }

    // Eliminar un observador.
    void eliminarObservador(Observador* observador) {

        observadores.erase(
            std::remove(
                observadores.begin(),
                observadores.end(),
                observador
            ),
            observadores.end()
        );
    }

    // Notificar a todos los observadores.
    void notificar(
        int vida,
        int puntos,
        int nivel,
        int cargas
    ) {

        for (Observador* observador : observadores) {

            observador->actualizarEstado(
                vida,
                puntos,
                nivel,
                cargas
            );
        }
    }
};
