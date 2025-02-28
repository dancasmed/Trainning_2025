#ifndef LINE
#define LINE
#include <memory>
#include <SFML/Graphics.hpp>

class Line {
public:
    std::shared_ptr<sf::Vector2f> v1, v2;
    sf::Color color;

    Line(std::shared_ptr<sf::Vector2f> a, std::shared_ptr<sf::Vector2f> b, sf::Color c);
    void Draw(sf::RenderWindow* window);
    float calculateSlope();
};

#endif