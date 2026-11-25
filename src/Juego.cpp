#include "Juego.hpp"
#include "ArmaElectrica.hpp"

#include <algorithm>
#include <functional>
#include <string>
#include <cmath>
#include <cstdio>
#include <iostream>

std::string Juego::formatearTiempo(double segundos) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.2f s", segundos);
    return std::string(buffer);
}

// ========================================
// CONSTRUCTOR
// ========================================

Juego::Juego()
    : ventana(
        sf::VideoMode({800, 600}),
        "Escape del Laboratorio"
      ),
      camara(sf::FloatRect({0.f, 0.f}, {800.f, 600.f})),
      vistaInterfaz(sf::FloatRect({0.f, 0.f}, {800.f, 600.f})),
      cientifico(),
      nivel(1),
      puntuacion(),
      tiempoAtaque(0.f),
      gameOver(false),
      victoria(false) {

    ventana.setFramerateLimit(60);

    // POO - OBSERVER: el HUD escucha cambios del juego.
    notificadorEstado.agregarObservador(&hud);
    notificarCambiosHUD();

    // Crear los enemigos exclusivos del nivel actual.
    crearEnemigosDelNivel();

    // POO - COMPOSICION:
    // Juego administra los objetos recolectables.
    objetos.reserve(4);

    objetos.emplace_back(
        "Energia", 10, sf::Vector2f{200.f, 150.f}
    );
    objetos.emplace_back(
        "Energia", 10, sf::Vector2f{1000.f, 150.f}
    );
    objetos.emplace_back(
        "Energia", 10, sf::Vector2f{200.f, 450.f}
    );
    objetos.emplace_back(
        "Energia", 10, sf::Vector2f{1000.f, 450.f}
    );

    // Cargar fuente.
    fuenteCargada = fuenteMenu.openFromFile(
        "C:/Windows/Fonts/arial.ttf"
    );

    if (!fuenteCargada) {
        fuenteCargada = fuenteMenu.openFromFile(
            "C:/Windows/Fonts/segoeui.ttf"
        );
    }

    // POO - COMPOSICION:
    // El fondo del menu pertenece a Juego.
    if (texturaFondoMenu.loadFromFile(
        "assets/imagenes/fondo_menu.png"
    )) {

        spriteFondoMenu.emplace(texturaFondoMenu);

        sf::Vector2u tamano = texturaFondoMenu.getSize();

        if (tamano.x > 0 && tamano.y > 0) {
            spriteFondoMenu->setScale({
                800.f / static_cast<float>(tamano.x),
                600.f / static_cast<float>(tamano.y)
            });
        }
    }

    irAMenu();
    aplicarCambioEstado();
    opcionMenu = 0;
    opcionResultado = 0;
    partidaIniciada = false;
}

// ========================================
// POSICIONES Y PATRULLAS SEGURAS
// ========================================

bool Juego::posicionEnemigoValida(sf::Vector2f posicion) const {
    return nivel.esTransitable(posicion, {40.f, 40.f});
}

bool Juego::patrullaValida(
    sf::Vector2f inicio,
    sf::Vector2f final
) const {
    const sf::Vector2f desplazamiento = final - inicio;
    const float distancia = std::sqrt(
        desplazamiento.x * desplazamiento.x +
        desplazamiento.y * desplazamiento.y
    );

    const int pasos = std::max(1, static_cast<int>(std::ceil(distancia / 5.f)));
    for (int i = 0; i <= pasos; ++i) {
        const float t = static_cast<float>(i) / static_cast<float>(pasos);
        const sf::Vector2f punto = inicio + desplazamiento * t;
        if (!posicionEnemigoValida(punto)) {
            return false;
        }
    }
    return true;
}

// ========================================
// FACTORY METHOD: ENEMIGOS POR NIVEL
// ========================================

void Juego::crearEnemigosDelNivel() {
    robot = nullptr;
    guardianes.clear();
    atacantes.clear();
    laseres.clear();
    enemigos.clear();

    enemigos.reserve(6);
    guardianes.reserve(2);
    atacantes.reserve(3);

    {
        FabricaPerseguidor fabrica;
        auto creado = fabrica.crear(cientifico);
        robot = dynamic_cast<RobotPerseguidor*>(creado.get());
        enemigos.push_back(std::move(creado));
        robot->establecerNivel(nivel);
    }

    auto agregarGuardian = [&](sf::Vector2f inicio, sf::Vector2f fin) {
        if (!patrullaValida(inicio, fin)) {
            std::cerr << "[MAPA] Patrulla de guardian bloqueada en nivel "
                      << nivel.getNumero() << "\n";
            return;
        }
        FabricaGuardian fabrica(inicio, fin);
        auto creado = fabrica.crear(cientifico);
        auto* guardian = dynamic_cast<RobotGuardian*>(creado.get());
        guardian->establecerNivel(nivel);
        guardian->establecerDificultad(nivel.getNumero());
        guardianes.push_back(guardian);
        enemigos.push_back(std::move(creado));
    };

    auto agregarAtacante = [&](sf::Vector2f posicion) {
        if (!posicionEnemigoValida(posicion)) {
            std::cerr << "[MAPA] Posicion de atacante bloqueada en nivel "
                      << nivel.getNumero() << "\n";
            return;
        }
        FabricaAtaque fabrica(posicion);
        auto creado = fabrica.crear(cientifico);
        atacantes.push_back(dynamic_cast<RobotAtaque*>(creado.get()));
        enemigos.push_back(std::move(creado));
    };

    if (nivel.getNumero() == 2) {
        agregarGuardian({320.f, 580.f}, {520.f, 580.f});
        agregarGuardian({805.f, 125.f}, {1005.f, 125.f});
    }
    else if (nivel.getNumero() == 3) {
        agregarGuardian({325.f, 365.f}, {525.f, 365.f});
        agregarGuardian({745.f, 645.f}, {945.f, 645.f});

        agregarAtacante({900.f, 220.f});
        agregarAtacante({680.f, 445.f});
        agregarAtacante({240.f, 680.f});
    }

    laseres.resize(atacantes.size());
}

// ========================================
// REINICIAR PARTIDA
// ========================================

void Juego::reiniciarPartida(bool reiniciarCronometro) {
    tiempoTutorial = 0.f;
    cientifico.reiniciar();

    crearEnemigosDelNivel();
    puntuacion.reiniciar();

    objetos.clear();
    objetos.emplace_back("Energia", 10, sf::Vector2f{200.f, 150.f});
    objetos.emplace_back("Energia", 10, sf::Vector2f{1000.f, 150.f});
    objetos.emplace_back("Energia", 10, sf::Vector2f{200.f, 450.f});
    objetos.emplace_back("Energia", 10, sf::Vector2f{1000.f, 450.f});

    nivel.setPuertaDesbloqueada(false);

    tiempoAtaque = 0.f;
    cargasElectricas = 5;
    tiempoDesdeDisparo = 1.f;
    armaElectrica.reiniciar();
    gameOver = false;
    victoria = false;

    if (reiniciarCronometro) {
        tiempoTotalPartida = 0.0;
        apiSincronizada = false;
        rankingGlobal.clear();
    }

    opcionResultado = 0;
    partidaIniciada = true;

    actualizarCamara();

    irAJugando();
    actualizarTitulo();

    reloj.restart();
}

// ========================================
// EVENTOS
// ========================================

void Juego::procesarEventos() {
    while (auto evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
            break;
        }

        if (estadoActual) {
            estadoActual->procesarEventos(*this, *evento);
            aplicarCambioEstado();
        }

        if (!ventana.isOpen()) {
            break;
        }
    }
}

void Juego::eventosMenu(sf::Event& evento) {
    // Permitir escribir nombre del jugador directamente en el menu
    if (const auto* textoIngresado = evento.getIf<sf::Event::TextEntered>()) {
        char32_t c = textoIngresado->unicode;
        if (c == 8) { // Backspace
            if (!nombreEditado) {
                nombreJugador.clear();
                nombreEditado = true;
            } else if (!nombreJugador.empty()) {
                nombreJugador.pop_back();
            }
        } else if (c >= 32 && c < 127) {
            if (!nombreEditado) {
                nombreJugador.clear();
                nombreEditado = true;
            }
            if (nombreJugador.size() < 15) {
                nombreJugador.push_back(static_cast<char>(c));
            }
        }
        return;
    }

    const auto* tecla = evento.getIf<sf::Event::KeyPressed>();
    if (!tecla) return;

    if (tecla->code == sf::Keyboard::Key::Up) {
        opcionMenu = (opcionMenu + 2) % 3;
    }
    else if (tecla->code == sf::Keyboard::Key::Down) {
        opcionMenu = (opcionMenu + 1) % 3;
    }
    else if (tecla->code == sf::Keyboard::Key::Enter) {
        if (opcionMenu == 0) {
            if (gameOver || victoria) {
                nivel.cambiarNivel(1);
                reiniciarPartida(true);
            } else {
                if (!partidaIniciada) {
                    tiempoTotalPartida = 0.0;
                }
                partidaIniciada = true;
                irAJugando();
                actualizarCamara();
                actualizarTitulo();
            }
        }
        else if (opcionMenu == 1) {
            irAControles();
        }
        else if (opcionMenu == 2) {
            ventana.close();
        }
    }
}

void Juego::eventosJugando(sf::Event& evento) {
    if (const auto* tecla = evento.getIf<sf::Event::KeyReleased>()) {
        if (tecla->code == sf::Keyboard::Key::P) {
            irAPausa();
            return;
        }
    }

    const auto* tecla = evento.getIf<sf::Event::KeyPressed>();
    if (!tecla) return;

    if (tecla->code == sf::Keyboard::Key::Space) {
        dispararElectrico();
    }
    else if (tecla->code == sf::Keyboard::Key::Escape) {
        irAMenu();
        ventana.setTitle("Escape del Laboratorio | Menu");
    }
}

void Juego::eventosPausa(sf::Event& evento) {
    const auto* tecla = evento.getIf<sf::Event::KeyReleased>();
    if (!tecla) return;

    if (tecla->code == sf::Keyboard::Key::P) {
        irAJugando();
    }
    else if (tecla->code == sf::Keyboard::Key::Escape) {
        irAMenu();
        ventana.setTitle("Escape del Laboratorio | Menu");
    }
}

void Juego::eventosControles(sf::Event& evento) {
    const auto* tecla = evento.getIf<sf::Event::KeyPressed>();
    if (!tecla) return;

    if (tecla->code == sf::Keyboard::Key::Escape ||
        tecla->code == sf::Keyboard::Key::Enter) {
        irAMenu();
        ventana.setTitle("Escape del Laboratorio | Menu");
    }
}

void Juego::eventosResultado(sf::Event& evento, bool gano) {
    const auto* tecla = evento.getIf<sf::Event::KeyPressed>();
    if (!tecla) return;

    if (tecla->code == sf::Keyboard::Key::Up ||
        tecla->code == sf::Keyboard::Key::Down) {
        opcionResultado = (opcionResultado + 1) % 2;
    }
    else if (tecla->code == sf::Keyboard::Key::Enter) {
        if (opcionResultado == 0) {
            if (gano) {
                if (nivel.getNumero() < 3) {
                    // Avanzar al siguiente nivel conservando el tiempo acumulado
                    nivel.cambiarNivel(nivel.getNumero() + 1);
                    reiniciarPartida(false);
                } else {
                    // Nueva partida desde Nivel 1: reiniciar cronometro
                    nivel.cambiarNivel(1);
                    reiniciarPartida(true);
                }
            } else {
                // Reintentar nivel actual: el cronometro global sigue acumulando
                reiniciarPartida(false);
            }
        } else {
            irAMenu();
            opcionMenu = 0;
            ventana.setTitle("Escape del Laboratorio | Menu");
        }
    }
    else if (tecla->code == sf::Keyboard::Key::Escape) {
        irAMenu();
        opcionMenu = 0;
        ventana.setTitle("Escape del Laboratorio | Menu");
    }
}

// ========================================
// POO - OBSERVER: NOTIFICACION DE CAMBIOS
// ========================================
void Juego::notificarCambiosHUD() {
    std::array<int, 4> estadoActual = {
        cientifico.getVida(),
        puntuacion.getPuntos(),
        nivel.getNumero(),
        cargasElectricas
    };

    if (!estadoHUDInicializado || estadoActual != ultimoEstadoHUD) {
        notificadorEstado.notificar(
            estadoActual[0], estadoActual[1],
            estadoActual[2], estadoActual[3]
        );
        ultimoEstadoHUD = estadoActual;
        estadoHUDInicializado = true;
    }
}

// ========================================
// ACTUALIZACION
// ========================================

void Juego::actualizar(float dt) {
    if (estadoActual) {
        estadoActual->actualizar(*this, dt);
        aplicarCambioEstado();
    }
}

void Juego::actualizarPartida(float dt) {

    if (gameOver || victoria) {
        return;
    }

    // Acumular tiempo total de toda la partida (Niveles 1 -> 2 -> 3)
    tiempoTotalPartida += static_cast<double>(dt);

    if (nivel.getNumero() == 1) {
        tiempoTutorial += dt;
    }

    sf::Vector2f posicionAnterior =
        cientifico.getPosicion();

    std::vector<std::reference_wrapper<Personaje>> personajes = {
        cientifico, *robot
    };
    for (Personaje& personaje : personajes) {
        personaje.update(dt);
    }

    if (nivel.getNumero() >= 2) {
        for (RobotGuardian* guardia : guardianes) {
            Personaje& personaje = *guardia;
            personaje.update(dt);
        }
    }

    if (nivel.getNumero() == 3) {
        for (std::size_t i = 0; i < atacantes.size(); ++i) {
            RobotAtaque* atacante = atacantes[i];
            EfectoLaser& laser = laseres[i];

            Personaje& personaje = *atacante;
            personaje.update(dt);
            laser.tiempoVisible = std::max(0.f, laser.tiempoVisible - dt);

            if (!atacante->consumirDisparo()) {
                continue;
            }

            const sf::Vector2f origen =
                atacante->getPosicion() + sf::Vector2f{20.f, 20.f};
            const sf::Vector2f destino =
                cientifico.getPosicion() + sf::Vector2f{20.f, 20.f};
            const sf::Vector2f diferencia = destino - origen;
            const float distancia = std::sqrt(
                diferencia.x * diferencia.x + diferencia.y * diferencia.y
            );

            bool bloqueado = false;
            laser.inicio = origen;
            laser.fin = destino;

            if (distancia > 0.f) {
                const sf::Vector2f direccion = diferencia / distancia;
                for (float paso = 5.f; paso < distancia; paso += 5.f) {
                    const sf::Vector2f punto = origen + direccion * paso;
                    const sf::FloatRect zonaLaser(
                        {punto.x - 2.f, punto.y - 2.f}, {4.f, 4.f}
                    );
                    if (nivel.hayColision(zonaLaser)) {
                        bloqueado = true;
                        laser.fin = punto;
                        break;
                    }
                }
            }

            laser.tiempoVisible = 0.2f;
            if (!bloqueado) {
                cientifico.recibirDanio(10);
            }
        }
    }

    // Colision del cientifico con paredes.
    if (nivel.hayColision(cientifico.getLimites())) {

        sf::Vector2f posicionNueva =
            cientifico.getPosicion();

        cientifico.setPosicion({
            posicionNueva.x,
            posicionAnterior.y
        });

        if (nivel.hayColision(cientifico.getLimites())) {

            cientifico.setPosicion({
                posicionAnterior.x,
                posicionNueva.y
            });

            if (nivel.hayColision(
                cientifico.getLimites()
            )) {

                cientifico.setPosicion(posicionAnterior);
            }
        }
    }

    // Recolectar baterias.
    for (Objeto& objeto : objetos) {

        if (objeto.estaRecogido()) {
            continue;
        }

        bool colision =
            cientifico.getLimites()
                .findIntersection(objeto.getLimites())
                .has_value();

        if (colision) {
            objeto.recoger();
            puntuacion.agregarPuntos(objeto.getValor());
        }
    }

    // Dano por contacto: perseguidor y ambos guardianes.
    tiempoAtaque += dt;
    bool peligro =
        cientifico.getLimites().findIntersection(robot->getLimites()).has_value()
        && !robot->estaAturdido();

    if (nivel.getNumero() >= 2) {
        for (RobotGuardian* guardia : guardianes) {
            if (!guardia->estaAturdido() &&
                cientifico.getLimites()
                    .findIntersection(guardia->getLimites()).has_value()) {
                peligro = true;
            }
        }
    }

    if (peligro && tiempoAtaque >= intervaloAtaque) {
        cientifico.recibirDanio(10);
        tiempoAtaque = 0.f;
    }

    // Comprobar estado de la puerta.
    nivel.setPuertaDesbloqueada(
        puntuacion.getPuntos() >= 40
    );

    // Verificar derrota.
    if (cientifico.getVida() <= 0) {
        gameOver = true;
        opcionResultado = 0;
        irAGameOver();
        actualizarTitulo();
        return;
    }

    // Verificar victoria.
    if (
        puntuacion.getPuntos() >= 40 &&
        nivel.tocarSalida(cientifico.getLimites())
    ) {
        nivel.completar();
        victoria = true;
        opcionResultado = 0;

        // Solo al completar los 3 niveles se sube el tiempo y se carga el Top 5
        if (nivel.getNumero() == 3) {
            sincronizarRankingGlobal();
        }

        irAVictoria();
        actualizarTitulo();
        return;
    }

    tiempoDesdeDisparo += dt;
    armaElectrica.actualizarEfecto(dt);

    actualizarCamara();
    actualizarTitulo();
}

// ========================================
// TITULO
// ========================================

void Juego::actualizarTitulo() {
    if (gameOver) {
        ventana.setTitle("Escape del Laboratorio | GAME OVER");
    }
    else if (victoria) {
        ventana.setTitle("Escape del Laboratorio | VICTORIA");
    }
    else {
        ventana.setTitle(
            "Escape del Laboratorio | Tiempo: " + formatearTiempo(tiempoTotalPartida)
        );
    }
}

// ========================================
// CAMARA
// ========================================

void Juego::actualizarCamara() {
    sf::Vector2f posicion = cientifico.getPosicion();

    sf::Vector2f centro = {
        posicion.x + 20.f,
        posicion.y + 20.f
    };

    float mitadAncho = 400.f;
    float mitadAlto = 300.f;

    centro.x = std::clamp(
        centro.x,
        mitadAncho,
        nivel.getAnchoMapa() - mitadAncho
    );

    centro.y = std::clamp(
        centro.y,
        mitadAlto,
        nivel.getAltoMapa() - mitadAlto
    );

    camara.setCenter(centro);
}

// ========================================
// MENU PRINCIPAL
// ========================================

void Juego::dibujarMenu() {

    ventana.setView(vistaInterfaz);

    if (spriteFondoMenu.has_value()) {
        ventana.draw(*spriteFondoMenu);
    } else {
        sf::RectangleShape fondo({800.f, 600.f});
        fondo.setFillColor(sf::Color(10, 18, 32));
        ventana.draw(fondo);
    }

    for (int x = 0; x <= 800; x += 40) {
        sf::RectangleShape linea({1.f, 600.f});
        linea.setPosition({static_cast<float>(x), 0.f});
        linea.setFillColor(sf::Color(30, 105, 125, 45));
        ventana.draw(linea);
    }

    for (int y = 0; y <= 600; y += 40) {
        sf::RectangleShape linea({800.f, 1.f});
        linea.setPosition({0.f, static_cast<float>(y)});
        linea.setFillColor(sf::Color(30, 105, 125, 45));
        ventana.draw(linea);
    }

    sf::RectangleShape panel({650.f, 470.f});
    panel.setPosition({75.f, 65.f});
    panel.setFillColor(sf::Color(10, 18, 32, 145));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(25, 190, 220));
    ventana.draw(panel);

    sf::RectangleShape barraSuperior({650.f, 12.f});
    barraSuperior.setPosition({75.f, 65.f});
    barraSuperior.setFillColor(sf::Color(10, 190, 220));
    ventana.draw(barraSuperior);

    sf::CircleShape alarma(9.f);
    alarma.setPosition({670.f, 98.f});
    alarma.setFillColor(sf::Color(230, 50, 65));
    ventana.draw(alarma);

    sf::RectangleShape separador({540.f, 2.f});
    separador.setPosition({130.f, 248.f});
    separador.setFillColor(sf::Color(35, 150, 180));
    ventana.draw(separador);

    const std::string opciones[3] = {
        gameOver || victoria
            ? "NUEVA PARTIDA"
            : (
                partidaIniciada
                    ? "CONTINUAR PARTIDA"
                    : "INICIAR PARTIDA"
              ),
        "CONTROLES",
        "SALIR"
    };

    for (int i = 0; i < 3; i++) {

        float y = 298.f + i * 62.f;

        sf::RectangleShape boton({390.f, 48.f});
        boton.setPosition({205.f, y});

        bool seleccionado = (opcionMenu == i);

        if (seleccionado) {
            boton.setFillColor(sf::Color(15, 105, 130));
            boton.setOutlineColor(sf::Color(65, 230, 245));
            boton.setOutlineThickness(2.f);
        } else {
            boton.setFillColor(sf::Color(22, 43, 63));
            boton.setOutlineColor(sf::Color(50, 95, 110));
            boton.setOutlineThickness(1.f);
        }

        ventana.draw(boton);

        if (seleccionado) {
            sf::RectangleShape indicador({7.f, 48.f});
            indicador.setPosition({205.f, y});
            indicador.setFillColor(sf::Color(90, 240, 255));
            ventana.draw(indicador);
        }

        if (fuenteCargada) {
            sf::Text texto(fuenteMenu, opciones[i], 21);
            texto.setFillColor(
                seleccionado
                    ? sf::Color(240, 255, 255)
                    : sf::Color(155, 195, 210)
            );

            sf::FloatRect limites = texto.getLocalBounds();
            texto.setPosition({
                400.f - limites.size.x / 2.f,
                y + 10.f
            });

            ventana.draw(texto);
        }
    }

    if (fuenteCargada) {
        sf::Text etiqueta(
            fuenteMenu,
            "SISTEMA DE SEGURIDAD - ACCESO RESTRINGIDO",
            14
        );
        etiqueta.setFillColor(sf::Color(235, 95, 100));
        etiqueta.setPosition({125.f, 100.f});
        ventana.draw(etiqueta);

        sf::Text titulo(fuenteMenu, "ESCAPE DEL", 41);
        titulo.setStyle(sf::Text::Bold);
        titulo.setFillColor(sf::Color(220, 245, 255));
        titulo.setPosition({145.f, 136.f});
        ventana.draw(titulo);

        sf::Text subtitulo(fuenteMenu, "LABORATORIO", 41);
        subtitulo.setStyle(sf::Text::Bold);
        subtitulo.setFillColor(sf::Color(50, 215, 240));
        subtitulo.setPosition({145.f, 183.f});
        ventana.draw(subtitulo);

        // Caja editable para el nombre del jugador en el ranking
        sf::RectangleShape cajaNombre({390.f, 32.f});
        cajaNombre.setPosition({205.f, 256.f});
        cajaNombre.setFillColor(sf::Color(15, 32, 50, 220));
        cajaNombre.setOutlineThickness(1.5f);
        cajaNombre.setOutlineColor(sf::Color(255, 215, 75));
        ventana.draw(cajaNombre);

        std::string nombreVisible = nombreJugador.empty() ? "" : nombreJugador;
        sf::Text textoJugador(
            fuenteMenu,
            "JUGADOR: " + nombreVisible + "|",
            16
        );
        textoJugador.setStyle(sf::Text::Bold);
        textoJugador.setFillColor(sf::Color(255, 225, 105));
        sf::FloatRect limJug = textoJugador.getLocalBounds();
        textoJugador.setPosition({
            400.f - limJug.size.x / 2.f,
            262.f
        });
        ventana.draw(textoJugador);

        sf::Text ayuda(
            fuenteMenu,
            "FLECHAS: SELECCIONAR    |    ENTER: CONFIRMAR",
            14
        );
        ayuda.setFillColor(sf::Color(145, 175, 190));
        ayuda.setPosition({195.f, 505.f});
        ventana.draw(ayuda);
    }
}

// ========================================
// CONTROLES
// ========================================

void Juego::dibujarControles() {

    ventana.setView(vistaInterfaz);

    sf::RectangleShape fondo({800.f, 600.f});
    fondo.setFillColor(sf::Color(10, 18, 32));
    ventana.draw(fondo);

    sf::RectangleShape panel({650.f, 470.f});
    panel.setPosition({75.f, 65.f});
    panel.setFillColor(sf::Color(16, 33, 52));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(35, 200, 225));
    ventana.draw(panel);

    if (!fuenteCargada) {
        return;
    }

    sf::Text titulo(fuenteMenu, "MANUAL DEL CIENTIFICO", 34);
    titulo.setStyle(sf::Text::Bold);
    titulo.setFillColor(sf::Color(70, 225, 245));
    titulo.setPosition({135.f, 105.f});
    ventana.draw(titulo);

    sf::Text subtitulo(fuenteMenu, "PROTOCOLO DE ESCAPE", 18);
    subtitulo.setFillColor(sf::Color(240, 100, 105));
    subtitulo.setPosition({135.f, 170.f});
    ventana.draw(subtitulo);

    const std::string instrucciones[5] = {
        "W / A / S / D   -   Mover al cientifico",
        "ESPACIO         -   Disparar arma electrica",
        "RECOGE          -   Las 4 baterias de energia",
        "ESCAPE          -   Supera los 3 niveles rapido",
        "RANKING         -   Menor tiempo entra al Top 5"
    };

    for (int i = 0; i < 5; i++) {
        sf::Text linea(fuenteMenu, instrucciones[i], 20);
        linea.setFillColor(sf::Color(225, 240, 245));
        linea.setPosition({130.f, 225.f + i * 48.f});
        ventana.draw(linea);
    }

    sf::Text regresar(fuenteMenu, "ESC / ENTER  -  VOLVER AL MENU", 17);
    regresar.setFillColor(sf::Color(75, 225, 245));
    regresar.setPosition({215.f, 488.f});
    ventana.draw(regresar);
}

// ========================================
// SINCRONIZACION API EXTERNA
// ========================================

void Juego::sincronizarRankingGlobal() {
    if (apiSincronizada) {
        return;
    }

    apiClient.enviarTiempo(
        nombreJugador,
        tiempoTotalPartida
    );

    rankingGlobal = apiClient.obtenerTop5();
    apiSincronizada = true;
}

// ========================================
// PANTALLA DE VICTORIA / DERROTA
// ========================================

void Juego::dibujarResultado(bool gano) {

    ventana.setView(vistaInterfaz);

    const bool juegoCompletado = gano && nivel.getNumero() == 3;

    sf::Color colorPrincipal = gano
        ? sf::Color(55, 225, 190)
        : sf::Color(245, 70, 90);

    sf::RectangleShape fondo({800.f, 600.f});
    fondo.setFillColor(sf::Color(10, 17, 30));
    ventana.draw(fondo);

    sf::RectangleShape franja({12.f, 600.f});
    franja.setFillColor(colorPrincipal);
    franja.setPosition({0.f, 0.f});
    ventana.draw(franja);
    franja.setPosition({788.f, 0.f});
    ventana.draw(franja);

    const std::string opciones[2] = {
        juegoCompletado
            ? "NUEVA PARTIDA"
            : (gano ? "SIGUIENTE NIVEL" : "REINTENTAR NIVEL"),
        "VOLVER AL MENU"
    };

    // ====================================================
    // CASO A: VICTORIA FINAL (3 NIVELES) -> CON TOP 5 GLOBAL
    // ====================================================
    if (juegoCompletado) {
        sf::RectangleShape panel({410.f, 450.f});
        panel.setPosition({35.f, 75.f});
        panel.setFillColor(sf::Color(18, 33, 50));
        panel.setOutlineThickness(3.f);
        panel.setOutlineColor(colorPrincipal);
        ventana.draw(panel);

        sf::RectangleShape barra({410.f, 10.f});
        barra.setPosition({35.f, 75.f});
        barra.setFillColor(colorPrincipal);
        ventana.draw(barra);

        sf::RectangleShape panelRanking({305.f, 450.f});
        panelRanking.setPosition({460.f, 75.f});
        panelRanking.setFillColor(sf::Color(14, 26, 42));
        panelRanking.setOutlineThickness(2.f);
        panelRanking.setOutlineColor(sf::Color(50, 195, 230));
        ventana.draw(panelRanking);

        sf::RectangleShape barraRanking({305.f, 10.f});
        barraRanking.setPosition({460.f, 75.f});
        barraRanking.setFillColor(sf::Color(50, 195, 230));
        ventana.draw(barraRanking);

        if (!fuenteCargada) return;

        const float centroIzq = 240.f;

        sf::Text titulo(fuenteMenu, "VICTORIA!", 42);
        titulo.setStyle(sf::Text::Bold);
        titulo.setFillColor(colorPrincipal);
        sf::FloatRect limT = titulo.getLocalBounds();
        titulo.setPosition({centroIzq - limT.size.x / 2.f, 115.f});
        ventana.draw(titulo);

        sf::Text mensaje(fuenteMenu, "MISION COMPLETADA (3/3)", 19);
        mensaje.setFillColor(sf::Color(220, 240, 245));
        sf::FloatRect limM = mensaje.getLocalBounds();
        mensaje.setPosition({centroIzq - limM.size.x / 2.f, 185.f});
        ventana.draw(mensaje);

        sf::Text tiempoTxt(
            fuenteMenu,
            "TIEMPO TOTAL: " + formatearTiempo(tiempoTotalPartida),
            21
        );
        tiempoTxt.setStyle(sf::Text::Bold);
        tiempoTxt.setFillColor(sf::Color(255, 220, 90));
        sf::FloatRect limP = tiempoTxt.getLocalBounds();
        tiempoTxt.setPosition({centroIzq - limP.size.x / 2.f, 235.f});
        ventana.draw(tiempoTxt);

        for (int i = 0; i < 2; i++) {
            float y = 300.f + i * 70.f;
            sf::RectangleShape boton({330.f, 52.f});
            boton.setPosition({75.f, y});

            bool seleccionado = (opcionResultado == i);
            boton.setFillColor(seleccionado ? sf::Color(45, 90, 105) : sf::Color(25, 45, 65));
            boton.setOutlineThickness(seleccionado ? 3.f : 1.f);
            boton.setOutlineColor(seleccionado ? colorPrincipal : sf::Color(75, 100, 120));
            ventana.draw(boton);

            sf::Text texto(fuenteMenu, opciones[i], 20);
            texto.setFillColor(sf::Color::White);
            sf::FloatRect limB = texto.getLocalBounds();
            texto.setPosition({centroIzq - limB.size.x / 2.f, y + 12.f});
            ventana.draw(texto);
        }

        sf::Text ayuda(fuenteMenu, "FLECHAS: SELECCIONAR   ENTER: CONFIRMAR", 13);
        ayuda.setFillColor(sf::Color(150, 185, 200));
        sf::FloatRect limA = ayuda.getLocalBounds();
        ayuda.setPosition({centroIzq - limA.size.x / 2.f, 475.f});
        ventana.draw(ayuda);

        // Panel Derecho: TOP 5 MENOR TIEMPO
        const float centroDer = 612.5f;

        sf::Text tituloRanking(fuenteMenu, "TOP 5 TIEMPOS", 23);
        tituloRanking.setStyle(sf::Text::Bold);
        tituloRanking.setFillColor(sf::Color(80, 230, 255));
        sf::FloatRect limR = tituloRanking.getLocalBounds();
        tituloRanking.setPosition({centroDer - limR.size.x / 2.f, 112.f});
        ventana.draw(tituloRanking);

        sf::RectangleShape sepRanking({255.f, 2.f});
        sepRanking.setPosition({485.f, 152.f});
        sepRanking.setFillColor(sf::Color(40, 130, 160));
        ventana.draw(sepRanking);

        if (rankingGlobal.empty()) {
            sf::Text sinDatos(
                fuenteMenu,
                apiClient.huboError()
                    ? "Modo offline:\nsin conexion al servidor"
                    : "Sin registros aun",
                16
            );
            sinDatos.setFillColor(sf::Color(180, 195, 210));
            sinDatos.setPosition({490.f, 200.f});
            ventana.draw(sinDatos);
        } else {
            for (std::size_t i = 0; i < rankingGlobal.size(); ++i) {
                const auto& reg = rankingGlobal[i];
                float yFila = 172.f + static_cast<float>(i) * 56.f;

                sf::RectangleShape filaBg({265.f, 44.f});
                filaBg.setPosition({480.f, yFila});
                filaBg.setFillColor(i == 0 ? sf::Color(45, 75, 95) : sf::Color(22, 40, 60));
                filaBg.setOutlineThickness(1.f);
                filaBg.setOutlineColor(i == 0 ? sf::Color(255, 215, 75) : sf::Color(45, 85, 110));
                ventana.draw(filaBg);

                std::string linea =
                    "#" + std::to_string(i + 1) + " " +
                    reg.jugador + "  -  " + formatearTiempo(reg.tiempo);

                sf::Text textoFila(fuenteMenu, linea, 15);
                textoFila.setFillColor(i == 0 ? sf::Color(255, 225, 105) : sf::Color(225, 240, 245));
                textoFila.setPosition({490.f, yFila + 12.f});
                ventana.draw(textoFila);
            }

            sf::Text estadoApi(fuenteMenu, "Sincronizado con la nube", 13);
            estadoApi.setFillColor(sf::Color(90, 220, 160));
            sf::FloatRect limE = estadoApi.getLocalBounds();
            estadoApi.setPosition({centroDer - limE.size.x / 2.f, 475.f});
            ventana.draw(estadoApi);
        }
        return;
    }

    // ====================================================
    // CASO B: GAME OVER O NIVEL INTERMEDIO (1 o 2)
    // ====================================================
    sf::RectangleShape panel({640.f, 430.f});
    panel.setPosition({80.f, 85.f});
    panel.setFillColor(sf::Color(18, 33, 50));
    panel.setOutlineThickness(3.f);
    panel.setOutlineColor(colorPrincipal);
    ventana.draw(panel);

    sf::RectangleShape barra({640.f, 10.f});
    barra.setPosition({80.f, 85.f});
    barra.setFillColor(colorPrincipal);
    ventana.draw(barra);

    if (!fuenteCargada) {
        return;
    }

    sf::Text titulo(fuenteMenu, gano ? "VICTORIA!" : "GAME OVER", 50);
    titulo.setStyle(sf::Text::Bold);
    titulo.setFillColor(colorPrincipal);
    sf::FloatRect limiteTitulo = titulo.getLocalBounds();
    titulo.setPosition({400.f - limiteTitulo.size.x / 2.f, 130.f});
    ventana.draw(titulo);

    sf::Text mensaje(
        fuenteMenu,
        gano
            ? "Nivel " + std::to_string(nivel.getNumero()) + " superado. Continua tu escape."
            : "El robot atrapo al cientifico",
        22
    );
    mensaje.setFillColor(sf::Color(220, 240, 245));
    sf::FloatRect limiteMensaje = mensaje.getLocalBounds();
    mensaje.setPosition({400.f - limiteMensaje.size.x / 2.f, 215.f});
    ventana.draw(mensaje);

    sf::Text infoTiempo(
        fuenteMenu,
        "TIEMPO ACUMULADO: " + formatearTiempo(tiempoTotalPartida),
        21
    );
    infoTiempo.setFillColor(sf::Color(255, 220, 90));
    sf::FloatRect limitePuntos = infoTiempo.getLocalBounds();
    infoTiempo.setPosition({400.f - limitePuntos.size.x / 2.f, 270.f});
    ventana.draw(infoTiempo);

    for (int i = 0; i < 2; i++) {
        float y = 335.f + i * 70.f;
        sf::RectangleShape boton({380.f, 55.f});
        boton.setPosition({210.f, y});

        bool seleccionado = (opcionResultado == i);
        boton.setFillColor(seleccionado ? sf::Color(45, 90, 105) : sf::Color(25, 45, 65));
        boton.setOutlineThickness(seleccionado ? 3.f : 1.f);
        boton.setOutlineColor(seleccionado ? colorPrincipal : sf::Color(75, 100, 120));
        ventana.draw(boton);

        sf::Text texto(fuenteMenu, opciones[i], 22);
        texto.setFillColor(sf::Color::White);
        sf::FloatRect limiteTexto = texto.getLocalBounds();
        texto.setPosition({400.f - limiteTexto.size.x / 2.f, y + 12.f});
        ventana.draw(texto);
    }

    sf::Text ayuda(fuenteMenu, "FLECHAS: SELECCIONAR   ENTER: CONFIRMAR", 14);
    ayuda.setFillColor(sf::Color(150, 185, 200));
    ayuda.setPosition({215.f, 485.f});
    ventana.draw(ayuda);
}

// ========================================
// DIBUJAR JUEGO
// ========================================

void Juego::dibujar() {
    notificarCambiosHUD();

    ventana.clear(sf::Color(30, 35, 45));

    if (estadoActual) {
        estadoActual->dibujar(*this);
    }

    ventana.display();
}

void Juego::dibujarPartida() {

    ventana.setView(camara);

    nivel.draw(ventana);

    for (const Objeto& objeto : objetos) {
        objeto.draw(ventana);
    }

    std::vector<std::reference_wrapper<Personaje>> personajes = {
        cientifico, *robot
    };
    for (Personaje& personaje : personajes) {
        personaje.draw(ventana);
    }

    if (nivel.getNumero() >= 2) {
        for (RobotGuardian* guardia : guardianes) {
            Personaje& personaje = *guardia;
            personaje.draw(ventana);
        }
    }

    if (nivel.getNumero() == 3) {
        for (RobotAtaque* atacante : atacantes) {
            Personaje& personaje = *atacante;
            personaje.draw(ventana);
        }

        for (const EfectoLaser& laser : laseres) {
            if (laser.tiempoVisible <= 0.f) {
                continue;
            }
            sf::VertexArray rayo(sf::PrimitiveType::Lines, 2);
            rayo[0].position = laser.inicio;
            rayo[0].color = sf::Color(255, 60, 60);
            rayo[1].position = laser.fin;
            rayo[1].color = sf::Color(255, 120, 120);
            ventana.draw(rayo);

            sf::CircleShape impacto(5.f);
            impacto.setOrigin({5.f, 5.f});
            impacto.setPosition(laser.fin);
            impacto.setFillColor(sf::Color(255, 80, 80));
            ventana.draw(impacto);
        }
    }

    armaElectrica.actualizar(
        cientifico.getPosicion(),
        cientifico.getDireccion()
    );

    armaElectrica.dibujar(ventana);

    // ========================================
    // HUD - VIDA Y ENERGIA
    // ========================================

    ventana.setView(vistaInterfaz);

    hud.dibujar(ventana, fuenteCargada ? &fuenteMenu : nullptr,
                cientifico.getVida(), puntuacion.getPuntos(),
                nivel.getNumero(), cargasElectricas);

    // Cronometro global visible debajo del nivel
    if (fuenteCargada) {
        sf::Text textoCrono(
            fuenteMenu,
            "TIEMPO: " + formatearTiempo(tiempoTotalPartida),
            16
        );
        textoCrono.setStyle(sf::Text::Bold);
        textoCrono.setFillColor(sf::Color(255, 225, 105));
        sf::FloatRect limC = textoCrono.getLocalBounds();
        textoCrono.setPosition({400.f - limC.size.x / 2.f, 60.f});
        ventana.draw(textoCrono);
    }

    // ========================================
    // TUTORIAL - NIVEL 1
    // ========================================
    if (nivel.getNumero() == 1 && fuenteCargada &&
        (tiempoTutorial < 5.f ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H))) {

        ventana.setView(vistaInterfaz);

        sf::RectangleShape panelTutorial({560.f, 95.f});
        panelTutorial.setPosition({120.f, 490.f});
        panelTutorial.setFillColor(sf::Color(10, 20, 35, 225));
        panelTutorial.setOutlineThickness(2.f);
        panelTutorial.setOutlineColor(sf::Color(60, 210, 240));
        ventana.draw(panelTutorial);

        sf::Text tituloTutorial(fuenteMenu, "PROTOCOLO DE ENTRENAMIENTO", 18);
        tituloTutorial.setStyle(sf::Text::Bold);
        tituloTutorial.setFillColor(sf::Color(80, 230, 255));
        tituloTutorial.setPosition({145.f, 502.f});
        ventana.draw(tituloTutorial);

        sf::Text instrucciones1(fuenteMenu, "WASD: Mover   |   ESPACIO: Disparar", 15);
        instrucciones1.setFillColor(sf::Color::White);
        instrucciones1.setPosition({145.f, 530.f});
        ventana.draw(instrucciones1);

        sf::Text instrucciones2(fuenteMenu, "Recoge 4 baterias y llega a la salida.", 15);
        instrucciones2.setFillColor(sf::Color::White);
        instrucciones2.setPosition({145.f, 553.f});
        ventana.draw(instrucciones2);
    }
}

void Juego::dibujarPausa() {
    dibujarPartida();

    ventana.setView(vistaInterfaz);

    sf::RectangleShape fondo({800.f, 600.f});
    fondo.setFillColor(sf::Color(0, 0, 0, 180));
    ventana.draw(fondo);

    if (fuenteCargada) {
        sf::Text titulo(fuenteMenu, "PAUSA", 48);
        titulo.setFillColor(sf::Color::Cyan);
        titulo.setPosition({315.f, 220.f});
        ventana.draw(titulo);

        sf::Text mensaje(fuenteMenu, "P: Continuar | ESC: Menu", 22);
        mensaje.setPosition({260.f, 310.f});
        ventana.draw(mensaje);
    }
}

// ========================================
// BUCLE PRINCIPAL
// ========================================

void Juego::ejecutar() {

    while (ventana.isOpen()) {

        float dt = std::min(
            reloj.restart().asSeconds(),
            0.05f
        );

        procesarEventos();

        if (!ventana.isOpen()) {
            break;
        }

        actualizar(dt);
        dibujar();
    }
}

void Juego::dispararElectrico() {

    if (cargasElectricas <= 0) {
        return;
    }

    if (tiempoDesdeDisparo < intervaloDisparo) {
        return;
    }

    cargasElectricas--;
    tiempoDesdeDisparo = 0.f;

    armaElectrica.actualizar(
        cientifico.getPosicion(),
        cientifico.getDireccion()
    );

    armaElectrica.activarDisparo();

    sf::Vector2f origen = cientifico.getPosicion();
    int direccion = cientifico.getDireccion();

    auto estaEnAlcance = [&](sf::Vector2f destino) {
        float dx = destino.x - origen.x;
        float dy = destino.y - origen.y;
        bool direccionCorrecta = false;

        if (direccion == 0) {
            direccionCorrecta = dy > 0.f && dy <= alcanceElectrico &&
                                std::abs(dx) <= 45.f;
        } else if (direccion == 1) {
            direccionCorrecta = dx < 0.f && -dx <= alcanceElectrico &&
                                std::abs(dy) <= 45.f;
        } else if (direccion == 2) {
            direccionCorrecta = dx > 0.f && dx <= alcanceElectrico &&
                                std::abs(dy) <= 45.f;
        } else if (direccion == 3) {
            direccionCorrecta = dy < 0.f && -dy <= alcanceElectrico &&
                                std::abs(dx) <= 45.f;
        }

        if (!direccionCorrecta) return false;

        sf::Vector2f inicioRayo = origen + sf::Vector2f{20.f, 20.f};
        sf::Vector2f finRayo = destino + sf::Vector2f{20.f, 20.f};
        sf::Vector2f diferencia = finRayo - inicioRayo;
        float distancia = std::sqrt(
            diferencia.x * diferencia.x + diferencia.y * diferencia.y
        );
        if (distancia <= 0.f) return true;

        sf::Vector2f vectorDireccion = diferencia / distancia;
        for (float paso = 5.f; paso < distancia; paso += 5.f) {
            sf::Vector2f punto = inicioRayo + vectorDireccion * paso;
            sf::FloatRect zonaRayo(
                {punto.x - 2.f, punto.y - 2.f}, {4.f, 4.f}
            );
            if (nivel.hayColision(zonaRayo)) return false;
        }
        return true;
    };

    if (estaEnAlcance(robot->getPosicion())) {
        robot->aturdir(3.f);
    }

    if (nivel.getNumero() >= 2) {
        for (RobotGuardian* guardia : guardianes) {
            if (estaEnAlcance(guardia->getPosicion())) {
                guardia->aturdir(3.f);
            }
        }
    }

    if (nivel.getNumero() == 3) {
        for (RobotAtaque* atacante : atacantes) {
            if (estaEnAlcance(atacante->getPosicion())) {
                atacante->aturdir(3.f);
            }
        }
    }
}