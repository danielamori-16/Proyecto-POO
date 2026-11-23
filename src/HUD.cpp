
#include "HUD.hpp"
#include <algorithm>
#include <string>

// ========================================
// PATRON OBSERVER
// ========================================

// POO - POLIMORFISMO:
// HUD implementa el metodo de Observador.
// Cuando el juego notifica un cambio,
// el HUD guarda los nuevos valores.

void HUD::actualizarEstado(
    int vida,
    int puntos,
    int nivel,
    int cargas
) {
    vidaActual = vida;
    puntosActuales = puntos;
    nivelActual = nivel;
    cargasActuales = cargas;
}

// ========================================
// DIBUJAR HUD
// ========================================

void HUD::dibujar(
    sf::RenderWindow& ventana,
    const sf::Font* fuente,
    int vida,
    int energia,
    int numeroNivel,
    int cargas
) const {

    // ========================================
    // OBSERVER: UTILIZAR DATOS NOTIFICADOS
    // ========================================

    // Conservamos estos parametros para
    // mantener la compatibilidad con Juego.cpp.
    // Los valores mostrados provienen del
    // estado almacenado por Observer.

    (void)vida;
    (void)energia;
    (void)numeroNivel;
    (void)cargas;

    const int vidaMostrada = vidaActual;
    const int energiaMostrada = puntosActuales;
    const int nivelMostrado = nivelActual;
    const int cargasMostradas = cargasActuales;

    // ========================================
    // PANEL DEL ARMA ELECTRICA
    // ========================================

    if (fuente != nullptr) {

        sf::RectangleShape panelArma({280.f, 42.f});
        panelArma.setPosition({260.f, 95.f});

        panelArma.setFillColor(
            sf::Color(10, 20, 35, 215)
        );

        panelArma.setOutlineThickness(1.f);
        panelArma.setOutlineColor(
            sf::Color(70, 210, 245)
        );

        ventana.draw(panelArma);

        sf::Text textoArma(
            *fuente,
            "DISPAROS RESTANTES: " +
                std::to_string(cargasMostradas) +
                " / 5",
            16
        );

        textoArma.setFillColor(
            sf::Color(100, 230, 255)
        );

        textoArma.setPosition({272.f, 108.f});
        ventana.draw(textoArma);
    }

    // ========================================
    // NUMERO DE NIVEL
    // ========================================

    if (fuente != nullptr) {

        sf::Text textoNivel(
            *fuente,
            "NIVEL " + std::to_string(nivelMostrado),
            20
        );

        textoNivel.setStyle(sf::Text::Bold);

        textoNivel.setFillColor(
            sf::Color(90, 230, 245)
        );

        sf::FloatRect limites =
            textoNivel.getLocalBounds();

        textoNivel.setPosition({
            400.f - limites.size.x / 2.f,
            30.f
        });

        ventana.draw(textoNivel);
    }

    // ========================================
    // FONDO Y BARRA DE VIDA
    // ========================================

    sf::RectangleShape fondoVida({200.f, 20.f});
    fondoVida.setPosition({28.f, 54.f});
    fondoVida.setFillColor(sf::Color(65, 75, 85));

    sf::RectangleShape barraVida({200.f, 20.f});
    barraVida.setPosition({28.f, 54.f});

    float porcentajeVida = std::clamp(
        static_cast<float>(vidaMostrada) / 100.f,
        0.f,
        1.f
    );

    barraVida.setSize({
        200.f * porcentajeVida,
        20.f
    });

    // ========================================
    // PANEL DE VIDA
    // ========================================

    sf::RectangleShape panelVida({255.f, 78.f});

    panelVida.setPosition({12.f, 12.f});

    panelVida.setFillColor(
        sf::Color(10, 20, 35, 215)
    );

    panelVida.setOutlineThickness(1.f);

    panelVida.setOutlineColor(
        sf::Color(65, 185, 210)
    );

    ventana.draw(panelVida);

    // ========================================
    // PANEL DE PUNTOS
    // ========================================

    sf::RectangleShape panelPuntos({190.f, 78.f});

    panelPuntos.setPosition({598.f, 12.f});

    panelPuntos.setFillColor(
        sf::Color(10, 20, 35, 215)
    );

    panelPuntos.setOutlineThickness(1.f);

    panelPuntos.setOutlineColor(
        sf::Color(235, 195, 65)
    );

    ventana.draw(panelPuntos);

    // ========================================
    // COLOR DE LA BARRA SEGUN LA VIDA
    // ========================================

    if (vidaMostrada > 60) {

        barraVida.setFillColor(
            sf::Color(60, 215, 120)
        );

    }
    else if (vidaMostrada > 30) {

        barraVida.setFillColor(
            sf::Color(240, 190, 45)
        );

    }
    else {

        barraVida.setFillColor(
            sf::Color(235, 65, 65)
        );
    }

    ventana.draw(fondoVida);
    ventana.draw(barraVida);

    // ========================================
    // TEXTOS DE VIDA Y ENERGIA
    // ========================================

    if (fuente != nullptr) {

        sf::Text textoVida(
            *fuente,
            "VIDA: " +
                std::to_string(vidaMostrada) +
                " / 100",
            19
        );

        textoVida.setStyle(sf::Text::Bold);

        textoVida.setFillColor(
            sf::Color::White
        );

        textoVida.setPosition({28.f, 22.f});
        ventana.draw(textoVida);

        sf::Text textoPuntos(
            *fuente,
            "ENERGIA: " +
                std::to_string(energiaMostrada) +
                " / 40",
            19
        );

        textoPuntos.setStyle(sf::Text::Bold);

        textoPuntos.setFillColor(
            sf::Color(255, 220, 80)
        );

        textoPuntos.setPosition({610.f, 35.f});
        ventana.draw(textoPuntos);
    }
}
