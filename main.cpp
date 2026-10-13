#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow ventana(
        sf::VideoMode({800, 600}),
        "Escape del Laboratorio"
    );

    ventana.setFramerateLimit(60);

    // Nuestro científico (por ahora, un rectángulo)
    sf::RectangleShape cientifico({40.f, 50.f});
    cientifico.setFillColor(sf::Color(80, 200, 255));
    cientifico.setPosition({100.f, 250.f});

    float velocidad = 4.f;

    while (ventana.isOpen()) {

        // 1. Procesar eventos
        while (const auto evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }
        }

        // 2. Mover al científico
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            cientifico.move({0.f, -velocidad});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            cientifico.move({0.f, velocidad});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            cientifico.move({-velocidad, 0.f});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            cientifico.move({velocidad, 0.f});
        }

        // 3. Dibujar
        ventana.clear(sf::Color(20, 25, 40));
        ventana.draw(cientifico);
        ventana.display();
    }

    return 0;
}