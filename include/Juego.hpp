#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <memory>
#include <array>
#include <string>

#include "Cientifico.hpp"
#include "FabricaRobots.hpp"
#include "Nivel.hpp"
#include "Objeto.hpp"
#include "Puntuacion.hpp"
#include "ArmaElectrica.hpp"
#include "HUD.hpp"
#include "NotificadorEstado.hpp"
#include "EstadoJuego.hpp"
#include "APIClient.hpp"

class Juego {
private:

    // PATRON DE DISENO: STATE
    std::unique_ptr<EstadoJuego> estadoActual;
    std::unique_ptr<EstadoJuego> estadoPendiente;

    friend class EstadoMenu;
    friend class EstadoJugando;
    friend class EstadoPausa;
    friend class EstadoGameOver;
    friend class EstadoVictoria;
    friend class EstadoControles;

    ArmaElectrica armaElectrica;
    sf::RenderWindow ventana;
    sf::View camara;
    sf::View vistaInterfaz;
    Cientifico cientifico;

    // FACTORY METHOD + COMPOSICION: Juego es propietario de los robots.
    std::vector<std::unique_ptr<Robot>> enemigos;

    // Referencias no propietarias a los robots creados por las fabricas.
    RobotPerseguidor* robot = nullptr;
    std::vector<RobotGuardian*> guardianes;
    std::vector<RobotAtaque*> atacantes;

    Nivel nivel;
    Puntuacion puntuacion;
    std::vector<Objeto> objetos;

    // Cada atacante tiene un laser independiente.
    struct EfectoLaser {
        sf::Vector2f inicio{0.f, 0.f};
        sf::Vector2f fin{0.f, 0.f};
        float tiempoVisible = 0.f;
    };
    std::vector<EfectoLaser> laseres;

    HUD hud;
    NotificadorEstado notificadorEstado;
    std::array<int, 4> ultimoEstadoHUD{100, 0, 1, 5};
    bool estadoHUDInicializado = false;

    // ========================================
    // INTEGRACION API EXTERNA (RANKING SPEEDRUN)
    // ========================================
    APIClient apiClient{"https://6b00654c7fc363ccff0a2953.mockapi.io/ranking"};
    std::vector<RegistroRanking> rankingGlobal;
    bool apiSincronizada = false;
    std::string nombreJugador = "Cientifico";
    bool nombreEditado = false;
    double tiempoTotalPartida = 0.0;

    sf::Clock reloj;
    float tiempoAtaque;
    float tiempoTutorial = 0.f;

    const float intervaloAtaque = 1.f;
    int cargasElectricas = 5;
    float tiempoDesdeDisparo = 1.f;
    const float intervaloDisparo = 0.4f;
    const float alcanceElectrico = 180.f;
    bool gameOver;
    bool victoria;

    sf::Font fuenteMenu;
    bool fuenteCargada = false;
    sf::Texture texturaFondoMenu;
    std::optional<sf::Sprite> spriteFondoMenu;
    int opcionMenu = 0;
    int opcionResultado = 0;
    bool partidaIniciada = false;

    static std::string formatearTiempo(double segundos);
    void dispararElectrico();
    void procesarEventos();
    void actualizar(float dt);
    void dibujar();
    void actualizarTitulo();
    void actualizarCamara();
    void dibujarMenu();
    void dibujarControles();
    void dibujarResultado(bool gano);
    void sincronizarRankingGlobal();
    void reiniciarPartida(bool reiniciarCronometro = true);
    void notificarCambiosHUD();
    void crearEnemigosDelNivel();
    bool posicionEnemigoValida(sf::Vector2f posicion) const;
    bool patrullaValida(sf::Vector2f inicio, sf::Vector2f final) const;

    // Metodos del patron State
    void cambiarEstado(std::unique_ptr<EstadoJuego> nuevoEstado);
    void aplicarCambioEstado();

    void irAMenu();
    void irAJugando();
    void irAPausa();
    void irAGameOver();
    void irAVictoria();
    void irAControles();

    void eventosMenu(sf::Event& evento);
    void eventosJugando(sf::Event& evento);
    void eventosPausa(sf::Event& evento);
    void eventosControles(sf::Event& evento);
    void eventosResultado(sf::Event& evento, bool gano);

    void actualizarPartida(float dt);
    void dibujarPartida();
    void dibujarPausa();

public:
    Juego();
    void ejecutar();
};