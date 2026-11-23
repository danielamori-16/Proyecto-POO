#pragma once

#include <string>
#include <vector>

struct RegistroRanking {
    std::string jugador;
    double tiempo = 0.0;
};

// ========================================
// CRITERIO III: INTEGRACION API EXTERNA
// Ranking global por menor tiempo (3 niveles)
// ========================================
class APIClient {
private:
    std::string endpointUrl;
    long timeoutSegundos;
    mutable std::string ultimoError;

    static size_t escribirRespuestaCallback(
        void* contenido,
        size_t tamano,
        size_t cantidad,
        void* usuarioData
    );

    bool ejecutarPeticion(
        const std::string& metodo,
        const std::string& cuerpoJson,
        std::string& respuestaSalida
    ) const;

public:
    explicit APIClient(
        std::string url,
        long timeoutSeg = 10
    );

    // Envia el tiempo total de los 3 niveles al servidor (POST)
    bool enviarTiempo(
        const std::string& jugador,
        double tiempoSegundos
    );

    // Descarga y ordena el Top 5 de menor tiempo (GET)
    std::vector<RegistroRanking> obtenerTop5() const;

    bool huboError() const;
    std::string getUltimoError() const;
};