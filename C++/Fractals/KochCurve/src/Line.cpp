#include "Line.h"
#include <limits>

Line::Line(std::shared_ptr<sf::Vector2f> a, std::shared_ptr<sf::Vector2f> b, sf::Color c) {
    v1 = a;
    v2 = b;
    color = c;
}

void Line::Draw(sf::RenderWindow* window) {
    sf::Vertex line[] = {
        sf::Vertex(*v1, color),
        sf::Vertex(*v2, color)
    };
    window->draw(line, 2, sf::LineStrip);
    window->display();
}

float Line::calculateSlope() {
    float x1 = v1->x;
    float y1 = v1->y;
    float x2 = v2->x;
    float y2 = v2->y;
    
    if (x2 - x1 == 0) {
        return std::numeric_limits<float>::infinity();
    }
    return (y2 - y1) / (x2 - x1);
}