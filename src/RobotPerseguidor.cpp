#include "RobotPerseguidor.hpp"
#include "Nivel.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>

RobotPerseguidor::RobotPerseguidor(const Personaje& objetivo)
    : Robot("Robot Perseguidor", {100.f, 100.f}, 100.f, 10),
      objetivo(objetivo) {
    // POO - COMPOSICION: sprite y textura pertenecen al robot.
    if (textura.loadFromFile("assets/imagenes/robot_perseguidor_sheet.png")) {
        sprite.emplace(textura);
        sprite->setTextureRect(sf::IntRect({340, 15}, {205, 245}));
        sprite->setScale({40.f / 205.f, 40.f / 245.f});
        sprite->setPosition(posicion);
    } else {
        std::cerr << "[PERSEGUIDOR] No se pudo cargar la textura\n";
    }
}

void RobotPerseguidor::establecerNivel(const Nivel& nuevoNivel) {
    // POO - ASOCIACION: utiliza el nivel sin administrarlo.
    nivel = &nuevoNivel;
    if (nivel->getNumero() == 1) velocidad = 150.f;
    else if (nivel->getNumero() == 2) velocidad = 165.f;
    else velocidad = 180.f;
    ruta.clear();
    indiceRuta = 0;
    tiempoRecalculo = 0.f;
    tiempoBloqueado = 0.f;
    tieneDestinoRuta = false;
}

bool RobotPerseguidor::tramoDespejado(sf::Vector2f inicio,
                                      sf::Vector2f fin) const {
    if (!nivel) return false;
    sf::Vector2f dif = fin - inicio;
    float distancia = std::sqrt(dif.x * dif.x + dif.y * dif.y);
    int pasos = std::max(1, static_cast<int>(std::ceil(distancia / 2.f)));
    for (int i = 0; i <= pasos; ++i) {
        float t = static_cast<float>(i) / pasos;
        if (!nivel->esTransitable(inicio + dif * t, {40.f, 40.f}))
            return false;
    }
    return true;
}

void RobotPerseguidor::recalcularRuta() {
    if (!nivel) return;
    sf::Vector2f nuevoDestino = objetivo.getPosicion();
    std::vector<sf::Vector2f> nuevaRuta = buscador.calcularRuta(
        posicion, nuevoDestino, {40.f, 40.f}, *nivel);
    // Nunca perseguir una ruta antigua hacia donde estuvo el cientifico.
    ruta = std::move(nuevaRuta);
    indiceRuta = 0;
    ultimoDestinoRuta = nuevoDestino;
    tieneDestinoRuta = true;
    tiempoRecalculo = 0.f;
}


bool RobotPerseguidor::moverHacia(
    sf::Vector2f destino,
    float dt
) {
    if (!nivel || dt <= 0.f) {
        return false;
    }

    sf::Vector2f diferencia = destino - posicion;

    float distancia = std::sqrt(
        diferencia.x * diferencia.x +
        diferencia.y * diferencia.y
    );

    if (distancia <= 0.01f) {
        return false;
    }

    float paso = std::min(velocidad * dt, distancia);
    sf::Vector2f anterior = posicion;

    // POO - ENCAPSULAMIENTO:
    // El perseguidor controla su movimiento
    // sin permitir atravesar paredes.

    auto intentarEje = [&](bool ejeX) -> bool {

        sf::Vector2f candidata = posicion;

        float restante = ejeX
            ? destino.x - posicion.x
            : destino.y - posicion.y;

        if (std::abs(restante) <= 0.01f) {
            return false;
        }

        float avance = std::copysign(
            std::min(paso, std::abs(restante)),
            restante
        );

        if (ejeX) {
            candidata.x += avance;
        } else {
            candidata.y += avance;
        }

        if (!nivel->esTransitable(
            candidata,
            {40.f, 40.f}
        )) {
            return false;
        }

        setPosicion(candidata);

        if (ejeX) {
            direccion = avance < 0.f ? 1 : 2;
        } else {
            direccion = avance < 0.f ? 3 : 0;
        }

        return true;
    };

    bool movimientoRealizado = false;

    // Intentar primero el eje que mas
    // necesita desplazamiento.
    // Si no se puede, probar el otro eje.

    if (std::abs(diferencia.x) >=
        std::abs(diferencia.y)) {

        movimientoRealizado = intentarEje(true);

        if (!movimientoRealizado) {
            movimientoRealizado = intentarEje(false);
        }

    } else {

        movimientoRealizado = intentarEje(false);

        if (!movimientoRealizado) {
            movimientoRealizado = intentarEje(true);
        }
    }

    sf::Vector2f desplazamiento =
        posicion - anterior;

    // Comprobar si realmente avanzamos
    // hacia el punto de A*.
    float distanciaAntes =
        diferencia.x * diferencia.x +
        diferencia.y * diferencia.y;

    sf::Vector2f diferenciaDespues =
        destino - posicion;

    float distanciaDespues =
        diferenciaDespues.x * diferenciaDespues.x +
        diferenciaDespues.y * diferenciaDespues.y;

    return movimientoRealizado &&
           distanciaDespues < distanciaAntes - 0.0001f &&
           (
               desplazamiento.x * desplazamiento.x +
               desplazamiento.y * desplazamiento.y
           ) > 0.0001f;
}


void RobotPerseguidor::actualizarAnimacion(float dt) {
    tiempoAnimacion += dt;
    if (tiempoAnimacion >= 0.15f) {
        tiempoAnimacion = 0.f;
        frameActual = (frameActual + 1) % 3;
    }
    if (!sprite) return;
    static const std::array<std::array<sf::IntRect, 3>, 4> animaciones = {{
        {{ sf::IntRect({340, 15}, {205, 245}),
           sf::IntRect({620, 15}, {205, 245}),
           sf::IntRect({900, 15}, {205, 245}) }},
        {{ sf::IntRect({340, 280}, {205, 245}),
           sf::IntRect({620, 280}, {205, 245}),
           sf::IntRect({900, 280}, {205, 245}) }},
        {{ sf::IntRect({340, 545}, {205, 245}),
           sf::IntRect({620, 545}, {205, 245}),
           sf::IntRect({900, 545}, {205, 245}) }},
        {{ sf::IntRect({340, 800}, {205, 245}),
           sf::IntRect({620, 800}, {205, 245}),
           sf::IntRect({900, 800}, {205, 245}) }}
    }};
    sprite->setTextureRect(animaciones[direccion][frameActual]);
}

void RobotPerseguidor::update(float dt) {
    if (tiempoAturdido > 0.f) {
        tiempoAturdido = std::max(0.f, tiempoAturdido - dt);
        return;
    }
    if (!nivel || dt <= 0.f) return;

    sf::Vector2f destino = objetivo.getPosicion();
    sf::Vector2f diferencia = destino - posicion;
    float distancia = std::sqrt(diferencia.x * diferencia.x +
                                diferencia.y * diferencia.y);
    if (distancia > rangoDeteccion) return;

    // Persecucion inmediata cuando hay linea despejada PARA TODO EL CUERPO.
    // Primero comprobar que el cientifico este en zona transitable para
    // un robot de 40x40; si no, A* elegira una meta cercana alcanzable.
    if (nivel->esTransitable(destino, {40.f, 40.f}) &&
        tramoDespejado(posicion, destino)) {
        ruta.clear();
        indiceRuta = 0;
        tieneDestinoRuta = false;
        tiempoRecalculo = 0.f;
        tiempoBloqueado = 0.f;
        if (distancia > 2.f) {
            // Movimiento directo libre: dividirlo en pequeños pasos por
            // la trayectoria (sin meter el robot en paredes).
            float avance = std::min(velocidad * dt, distancia);
            sf::Vector2f candidato = posicion + diferencia / distancia * avance;
            if (nivel->esTransitable(candidato, {40.f, 40.f})) {
                sf::Vector2f anterior = posicion;
                setPosicion(candidato);
                sf::Vector2f real = posicion - anterior;
                if (std::abs(real.x) > std::abs(real.y))
                    direccion = real.x < 0.f ? 1 : 2;
                else direccion = real.y < 0.f ? 3 : 0;
                actualizarAnimacion(dt);
            }
        }
        return;
    }

    tiempoRecalculo += dt;
    float cambio = tieneDestinoRuta
        ? std::sqrt((destino.x - ultimoDestinoRuta.x) *
                    (destino.x - ultimoDestinoRuta.x) +
                    (destino.y - ultimoDestinoRuta.y) *
                    (destino.y - ultimoDestinoRuta.y))
        : 10000.f;

    // Respuesta rapida al jugador, pero sin recalcular en cada frame.
    bool requiereRuta = ruta.empty() || indiceRuta >= ruta.size() ||
                        !tieneDestinoRuta || cambio >= 25.f ||
                        tiempoRecalculo >= 0.45f;
    if (requiereRuta && tiempoRecalculo >= 0.22f)
        recalcularRuta();

    // Consumir los puntos realmente alcanzados.
    while (indiceRuta < ruta.size()) {
        sf::Vector2f delta = ruta[indiceRuta] - posicion;
        if (delta.x * delta.x + delta.y * delta.y <= 1.f)
            ++indiceRuta;
        else
            break;
    }
    if (indiceRuta >= ruta.size()) return;

    // El A* devuelve puntos en orden; NO saltarlos ni cortar paredes.
    if (moverHacia(ruta[indiceRuta], dt)) {
        tiempoBloqueado = 0.f;
        actualizarAnimacion(dt);
    } else {
        tiempoBloqueado += dt;
        if (tiempoBloqueado >= 0.50f) {
            // No repetir la misma ruta al mismo obstaculo indefinidamente.
            ruta.clear();
            indiceRuta = 0;
            tieneDestinoRuta = false;
            tiempoRecalculo = 0.22f;
            tiempoBloqueado = 0.f;
        }
    }
}

void RobotPerseguidor::draw(sf::RenderWindow& ventana) {
    if (sprite) {
        sprite->setPosition(posicion);
        ventana.draw(*sprite);
    } else {
        ventana.draw(figura);
    }
}

void RobotPerseguidor::atacar() {
    // Juego administra el dano por contacto.
}

void RobotPerseguidor::aturdir(float duracion) {
    tiempoAturdido = std::max(tiempoAturdido, duracion);
}

bool RobotPerseguidor::estaAturdido() const {
    return tiempoAturdido > 0.f;
}

void RobotPerseguidor::reiniciar() {
    tiempoAturdido = 0.f;
    vida = 100;
    setPosicion({100.f, 100.f});
    ruta.clear();
    indiceRuta = 0;
    tiempoRecalculo = 0.f;
    tiempoBloqueado = 0.f;
    ultimoDestinoRuta = {0.f, 0.f};
    tieneDestinoRuta = false;
    tiempoAnimacion = 0.f;
    frameActual = 0;
    direccion = 0;
    if (sprite) {
        sprite->setTextureRect(sf::IntRect({340, 15}, {205, 245}));
        sprite->setPosition(posicion);
    }
}
