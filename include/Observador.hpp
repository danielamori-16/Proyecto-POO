
#pragma once

// ========================================
// PATRON OBSERVER
// ========================================

// POO - ABSTRACCION:
// Define el comportamiento de los
// objetos que desean recibir notificaciones.

class Observador {
public:

    // Destructor virtual.
    virtual ~Observador() = default;

    // POO - POLIMORFISMO:
    // Cada observador decide como
    // reaccionar a los cambios.
    virtual void actualizarEstado(
        int vida,
        int puntos,
        int nivel,
        int cargas
    ) = 0;
};
