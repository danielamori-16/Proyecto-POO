
#pragma once

class Puntuacion {
private:
    int puntos;

public:
    Puntuacion();

    void agregarPuntos(int cantidad);
    int getPuntos() const;
    void reiniciar();
};
