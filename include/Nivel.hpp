#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

class Nivel {
private:
    int numero;
    bool completado;

    // POO - ENCAPSULAMIENTO:
    // Dimensiones privadas del escenario.
    const float anchoMapa = 1200.f;
    const float altoMapa = 800.f;

    std::vector<sf::RectangleShape> paredes;
    sf::RectangleShape salida;

    // POO - COMPOSICION:
    // Recursos visuales del escenario.
    sf::Texture texturaSuelo;
    std::optional<sf::Sprite> spriteSuelo;

    sf::Texture texturaPared;

    sf::Texture texturaPuerta;
    std::optional<sf::Sprite> spritePuertaBloqueada;
    std::optional<sf::Sprite> spritePuertaAbierta;

    bool puertaDesbloqueada = false;

public:
    Nivel(int numero);
    // POO - ENCAPSULAMIENTO:
    // Permite cambiar el mapa sin reemplazar el objeto Nivel.
    void cambiarNivel(int nuevoNumero);
    int getNumero() const;

    void crearEscenario();
    void draw(sf::RenderWindow& ventana) const;

    bool hayColision(const sf::FloatRect& limites) const;
    bool tocarSalida(const sf::FloatRect& limites) const;

    bool esTransitable(
        sf::Vector2f posicion,
        sf::Vector2f tamano
    ) const;

    float getAnchoMapa() const {
        return anchoMapa;
    }

    float getAltoMapa() const {
        return altoMapa;
    }

    void setPuertaDesbloqueada(bool desbloqueada);
    bool puertaEstaDesbloqueada() const;

    void completar();
    bool estaCompletado() const;
};