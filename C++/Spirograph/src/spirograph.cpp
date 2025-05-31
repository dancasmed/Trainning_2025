#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

// Función para generar los puntos de un espirógrafo
std::vector<sf::Vector2f> generateSpirograph(float R, float r, float d, int points = 5000) {
    std::vector<sf::Vector2f> spiroPoints;
    float t = 0.0;
    float step = 0.01;

    for (int i = 0; i < points; ++i) {
        float x = (R - r) * cos(t) + d * cos(((R - r) / r) * t);
        float y = (R - r) * sin(t) - d * sin(((R - r) / r) * t);
        spiroPoints.emplace_back(x, y);
        t += step;
    }

    return spiroPoints;
}

int main() {
    // Configuración de parámetros del espirógrafo
    float R = 180.0f;  // Radio del círculo externo
    float r = 50.0f;   // Radio del círculo interno
    float d = 160.0f;  // Distancia al lápiz

    auto points = generateSpirograph(R, r, d);

    // Configuración de la ventana
    sf::RenderWindow window(sf::VideoMode(800, 800), "Espirografo en SFML");
    window.setFramerateLimit(60);

    // Centrar los puntos en la ventana
    for (auto& point : points) {
        point.x += 400;
        point.y += 400;
    }

    // Dibujar el espirógrafo
    sf::VertexArray spirograph(sf::LineStrip, points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        spirograph[i].position = points[i];
        spirograph[i].color = sf::Color(255, 100, 200);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(spirograph);
        window.display();
    }

    return 0;
}
