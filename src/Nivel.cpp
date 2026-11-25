#include "Nivel.hpp"

#include <vector>
#include <string>
#include <iostream>

Nivel::Nivel(int numero)
    : numero(numero),
      completado(false),
      salida({50.f, 70.f}) {

    salida.setFillColor(sf::Color(0, 180, 255));
    salida.setPosition({1100.f, 680.f});

    // POO - COMPOSICION:
    // La textura y el sprite del suelo pertenecen a Nivel.
    if (!texturaSuelo.loadFromFile(
        "assets/imagenes/suelo_laboratorio.png"
    )) {
        std::cerr << "Error al cargar el suelo del laboratorio\n";
    } else {
        texturaSuelo.setRepeated(true);

        spriteSuelo.emplace(texturaSuelo);

        spriteSuelo->setTextureRect(
            sf::IntRect(
                {0, 0},
                {
                    static_cast<int>(anchoMapa),
                    static_cast<int>(altoMapa)
                }
            )
        );

        spriteSuelo->setPosition({0.f, 0.f});
    }

    // POO - COMPOSICION:
    // Nivel contiene la textura de las paredes.
    if (!texturaPared.loadFromFile(
        "assets/imagenes/pared_laboratorio.png"
    )) {
        std::cerr << "Error al cargar la textura de las paredes\n";
    } else {
        texturaPared.setRepeated(true);
    }

    // POO - COMPOSICION:
    // Nivel administra internamente los sprites de la puerta.
    if (!texturaPuerta.loadFromFile(
        "assets/imagenes/puertas_salida.png"
    )) {
        std::cerr << "Error al cargar la textura de la puerta\n";
    } else {
        sf::Vector2u tamano = texturaPuerta.getSize();

        if (tamano.x > 1 && tamano.y > 0) {
            unsigned int mitad = tamano.x / 2;
            unsigned int alto = tamano.y;

            spritePuertaBloqueada.emplace(texturaPuerta);
            spritePuertaAbierta.emplace(texturaPuerta);

            spritePuertaBloqueada->setTextureRect(
                sf::IntRect(
                    {0, 0},
                    {
                        static_cast<int>(mitad),
                        static_cast<int>(alto)
                    }
                )
            );

            spritePuertaAbierta->setTextureRect(
                sf::IntRect(
                    {static_cast<int>(mitad), 0},
                    {
                        static_cast<int>(mitad),
                        static_cast<int>(alto)
                    }
                )
            );

            float anchoDeseado = 110.f;
            float altoDeseado = 130.f;

            sf::Vector2f escala = {
                anchoDeseado / static_cast<float>(mitad),
                altoDeseado / static_cast<float>(alto)
            };

            spritePuertaBloqueada->setScale(escala);
            spritePuertaAbierta->setScale(escala);

            sf::Vector2f posicionPuerta = {
                salida.getPosition().x - 30.f,
                salida.getPosition().y - 30.f
            };

            spritePuertaBloqueada->setPosition(posicionPuerta);
            spritePuertaAbierta->setPosition(posicionPuerta);
        }
    }

    crearEscenario();
}


void Nivel::crearEscenario() {

    // POO - ENCAPSULAMIENTO:
    // Nivel administra internamente su mapa.
    // Juego no necesita conocer la estructura del laberinto.

    paredes.clear();

    const float tamCelda = 40.f;

    std::vector<std::string> mapa;

    // ========================================
    // NIVEL 1 - EL DESPERTAR
    // ========================================

    if (numero == 1) {

        // Conservamos nuestro mapa original.

        mapa = {
            "##############################",
            "#............................#",
            "#......##..........##........#",
            "#......##...##.....##........#",
            "#............................#",
            "#..###...#....###......###...#",
            "#........##........##........#",
            "#...##........##........##...#",
            "#...##........##........##...#",
            "#........##........##........#",
            "#..##....##...###......###...#",
            "#............................#",
            "#......##...##.........##....#",
            "#......##...##.........##....#",
            "#............................#",
            "#...####.......###......###..#",
            "#........##........##........#",
            "#....##........##........##..#",
            "#............................#",
            "##############################"
        };

    }

    // ========================================
    // NIVEL 2 - ALERTA DE SEGURIDAD
    // ========================================

    else if (numero == 2) {

        // Mas obstaculos y pasillos estrechos.
        // El jugador debera buscar rutas alternativas.

        mapa = {
            "##############################",
            "#............................#",
            "#....##......##......####....#",
            "#...........##...............#",
            "#.....#...........##.........#",
            "#.....#...####....##...###...#",
            "#.........#..................#",
            "#...##....#....###.....##....#",
            "#.........#...........##.....#",
            "#....##.......###...........##",
            "#.........#........###.......#",
            "#...#....##...###......##....#",
            "#...#..................##....#",
            "#......####....###...........#",
            "#..............###...###.....#",
            "#...###......................#",
            "#........##.....###...#......#",
            "#........##...........##.....#",
            "#............................#",
            "##############################"
        };

    }

    
    // ========================================
    // NIVEL 3 - ESCAPE FINAL
    // ========================================

    else if (numero == 3) {

        // Escenario con obstaculos distribuidos
        // y zonas abiertas para los enfrentamientos.

        mapa = {
            "##############################",
            "#............................#",
            "#...###..........####........#",
            "#............................#",
            "#........##...........###....#",
            "#............................#",
            "#...##........####...........#",
            "#............................#",
            "#........####........##......#",
            "#............................#",
            "#...####..............####...#",
            "#............................#",
            "#...........###..............#",
            "#............................#",
            "#..####...........###........#",
            "#............................#",
            "#........####................#",
            "#............................#",
            "#............................#",
            "##############################"
        };
    }
    // ========================================
    // CONSTRUIR LAS PAREDES DEL MAPA
    // ========================================

    // POO - COMPOSICION:
    // Cada pared se almacena dentro de Nivel
    // y forma parte de su escenario.

    for (int fila = 0;
         fila < static_cast<int>(mapa.size());
         fila++) {

        for (int columna = 0;
             columna < static_cast<int>(mapa[fila].size());
             columna++) {

            if (mapa[fila][columna] == '#') {

                sf::RectangleShape pared({
                    tamCelda,
                    tamCelda
                });

                pared.setPosition({
                    columna * tamCelda,
                    fila * tamCelda
                });

                // Utilizar la textura futurista.
                if (texturaPared.getSize().x > 0 &&
                    texturaPared.getSize().y > 0) {

                    pared.setTexture(&texturaPared);

                    pared.setTextureRect(
                        sf::IntRect(
                            {0, 0},
                            {
                                static_cast<int>(tamCelda),
                                static_cast<int>(tamCelda)
                            }
                        )
                    );

                    pared.setFillColor(sf::Color::White);

                } else {

                    pared.setFillColor(
                        sf::Color(100, 110, 130)
                    );
                }

                pared.setOutlineThickness(1.f);

                pared.setOutlineColor(
                    sf::Color(55, 65, 85)
                );

                paredes.push_back(pared);
            }
        }
    }
}


void Nivel::draw(sf::RenderWindow& ventana) const {
    if (spriteSuelo.has_value()) {
        ventana.draw(*spriteSuelo);
    }

    for (const auto& pared : paredes) {
        ventana.draw(pared);
    }

    if (spritePuertaBloqueada.has_value() &&
        spritePuertaAbierta.has_value()) {

        if (puertaDesbloqueada) {
            ventana.draw(*spritePuertaAbierta);
        } else {
            ventana.draw(*spritePuertaBloqueada);
        }

    } else {
        ventana.draw(salida);
    }
}


bool Nivel::hayColision(
    const sf::FloatRect& limites
) const {

    // POO - ENCAPSULAMIENTO:
    // Nivel administra las colisiones.

    for (const auto& pared : paredes) {

        // Usar el tamaño real de la pared,
        // sin incluir su borde decorativo.
        sf::FloatRect areaPared(
            pared.getPosition(),
            pared.getSize()
        );

        if (limites.findIntersection(
            areaPared
        ).has_value()) {

            return true;
        }
    }

    return false;
}


bool Nivel::esTransitable(
    sf::Vector2f posicion,
    sf::Vector2f tamano
) const {

    if (posicion.x < 0.f ||
        posicion.y < 0.f ||
        posicion.x + tamano.x > anchoMapa ||
        posicion.y + tamano.y > altoMapa) {
        return false;
    }

    sf::FloatRect area(posicion, tamano);

    return !hayColision(area);
}

bool Nivel::tocarSalida(
    const sf::FloatRect& limites
) const {

    return limites.findIntersection(
        salida.getGlobalBounds()
    ).has_value();
}

void Nivel::setPuertaDesbloqueada(bool desbloqueada) {
    puertaDesbloqueada = desbloqueada;
}

bool Nivel::puertaEstaDesbloqueada() const {
    return puertaDesbloqueada;
}

void Nivel::completar() {
    completado = true;
}

bool Nivel::estaCompletado() const {
    return completado;
}

void Nivel::cambiarNivel(int nuevoNumero) {

    // POO - ENCAPSULAMIENTO:
    // El nivel controla sus propios datos.
    numero = nuevoNumero;
    completado = false;
    puertaDesbloqueada = false;

    // Construir el laberinto del nuevo nivel.
    crearEscenario();
}

int Nivel::getNumero() const {
    return numero;
}
