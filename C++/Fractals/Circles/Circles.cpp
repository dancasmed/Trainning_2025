#include <SFML/Graphics.hpp>
#include <iostream>

void drawCircles(float x, float y, float radius, sf::RenderWindow *window)
{
    if (radius < 10)
    {
        return;
    }

    sf::CircleShape circle;    
    circle.setRadius(radius);
    circle.setOutlineColor(sf::Color::White);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(1);
    circle.setPosition(x - radius, y - radius);

    window->draw(circle);

    drawCircles(x + radius / 2, y, radius / 2, window);
    drawCircles(x - radius / 2, y, radius / 2, window);
    drawCircles(x, y + radius / 2, radius / 2, window);
    drawCircles(x, y - radius / 2, radius / 2, window);    
}

int main()
{   
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Fractals - Circles");
    
    float radius = 490;
    float circleX = 500;
    float circleY = 500;
    bool draw = true;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        if(draw) {
            drawCircles(circleX, circleY, radius, window);
            draw = false;
            window->display();
        }
    }

    return 0;
}