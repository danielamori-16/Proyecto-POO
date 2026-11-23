#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Nivel;

// POO - ABSTRACCION: A* calcula caminos transitables en el laberinto.
class BuscadorRutas {
private:
    // POO - ENCAPSULAMIENTO: cache de la cuadricula del nivel actual.
    mutable const Nivel* nivelCache = nullptr;
    mutable int numeroCache = -1;
    mutable int columnasCache = 0;
    mutable int filasCache = 0;
    mutable sf::Vector2f tamanoCache{0.f, 0.f};
    mutable std::vector<unsigned char> transitableCache;

public:
    std::vector<sf::Vector2f> calcularRuta(
        sf::Vector2f inicio,
        sf::Vector2f destino,
        sf::Vector2f tamanoRobot,
        const Nivel& nivel
    ) const;
};
