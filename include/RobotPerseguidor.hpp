#pragma once

#include "Robot.hpp"
#include "BuscadorRutas.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstddef>
#include <optional>

class Nivel;

// POO - HERENCIA: RobotPerseguidor hereda de Robot.
class RobotPerseguidor : public Robot {
private:
    // POO - COMPOSICION: recursos graficos y buscador A*.
    sf::Texture textura;
    std::optional<sf::Sprite> sprite;
    BuscadorRutas buscador;

    float tiempoAnimacion = 0.f;
    int frameActual = 0;
    int direccion = 0;

    // POO - ASOCIACION: referencias no propietarias.
    const Personaje& objetivo;
    const Nivel* nivel = nullptr;

    float rangoDeteccion = 2000.f;

    // POO - ENCAPSULAMIENTO: ruta, reaccion y aturdimiento.
    std::vector<sf::Vector2f> ruta;
    std::size_t indiceRuta = 0;
    float tiempoRecalculo = 0.f;
    float tiempoBloqueado = 0.f;
    float tiempoAturdido = 0.f;
    sf::Vector2f ultimoDestinoRuta{0.f, 0.f};
    bool tieneDestinoRuta = false;

    bool tramoDespejado(sf::Vector2f inicio, sf::Vector2f fin) const;
    void recalcularRuta();
    bool moverHacia(sf::Vector2f destino, float dt);
    void actualizarAnimacion(float dt);

public:
    explicit RobotPerseguidor(const Personaje& objetivo);
    void establecerNivel(const Nivel& nuevoNivel);
    void update(float dt) override;
    void draw(sf::RenderWindow& ventana) override;
    void atacar() override;
    void aturdir(float duracion);
    bool estaAturdido() const;
    void reiniciar();
};
