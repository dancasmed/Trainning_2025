#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

sf::RenderWindow *window;

class Line
{
    public:
        std::shared_ptr<sf::Vector2f> v1, v2;
        sf::Color color;

        Line(std::shared_ptr<sf::Vector2f> a, std::shared_ptr<sf::Vector2f> b, sf::Color c)
        {
            v1 = a;
            v2 = b;
            color = c;
        }

        void Draw(sf::RenderWindow *window)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(*v1, color),
                sf::Vertex(*v2, color)
            };

            window->draw(line, 2, sf::LineStrip);
            window->display();
        }

        float calculateSlope() {
    float x1 = v1->x;
    float y1 = v1->y;
    float x2 = v2->x;
    float y2 = v2->y;

    if (x2 - x1 == 0) {
        // La recta es vertical, la pendiente es infinita
        return std::numeric_limits<float>::infinity();
    } else {
        return (y2 - y1) / (x2 - x1);
    }
}
};

std::shared_ptr<sf::Vector2f> calcularPuntoC(std::shared_ptr<sf::Vector2f> p1, std::shared_ptr<sf::Vector2f> p2, float slope) {
    // Calcula el vector que va desde p1 a p2
     float vx = p2->x - p1->x;
    float vy = p2->y - p1->y;

    float rotationAngle = 60.0;

    if (slope < 0 ) rotationAngle = -60.0f;

    // Rota el vector 60 grados en sentido antihorario (para formar un triángulo equilátero)
    float cos60 = std::cos(rotationAngle * 3.14159f / 180.0f);
    float sin60 = std::sin(rotationAngle * 3.14159f / 180.0f);
    float vRotadoX = vx * cos60 - vy * sin60;
    float vRotadoY = vx * sin60 + vy * cos60;

    // El tercer punto es la suma del primer punto y el vector rotado
    return std::make_shared<sf::Vector2f>(p1->x + vRotadoX, p1->y + vRotadoY);
}

void processLine(std::vector< std::shared_ptr<Line> > *lines)
{
    auto line = lines->front();

    float slope = line->calculateSlope();

    float dx = line->v2->x - line->v1->x;// p2.x - p1.x;
    float dy = line->v2->y - line->v1->y;

    // Aplica la fórmula de la distancia euclidiana
    float d = std::sqrt(dx * dx + dy * dy);
    if ( d <10) {
        lines->erase(lines->begin());
        return;
    }
    float v3x = ((line->v2->x - line->v1->x) / 3) + line->v1->x;
    float v3y = ((line->v2->y - line->v1->y) / 3) + line->v1->y;
    
    float v4x = line->v2->x - ((line->v2->x - line->v1->x) / 3);
    float v4y = line->v2->y - ((line->v2->y - line->v1->y) / 3);
    
    
    auto v3 = std::make_shared<sf::Vector2f>(v3x, v3y);
    auto v4 = std::make_shared<sf::Vector2f>(v4x, v4y);

    auto v5 = calcularPuntoC(v3, v4, slope);

    lines->push_back(std::make_shared<Line>(v3, v5, sf::Color::Red));
    lines->push_back(std::make_shared<Line>(v4, v5, sf::Color::Red));
    lines->push_back(std::make_shared<Line>(line->v1, v3, sf::Color::Red));
    lines->push_back(std::make_shared<Line>(v4, line->v2, sf::Color::Red));
    
    //window->clear();
    lines->erase(lines->begin());
}


int main()
{   
    window = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Fractals - Koch Curve");
    
    std::shared_ptr<Line> l = std::make_shared<Line>(
        std::make_shared<sf::Vector2f>(0, 500),
        std::make_shared<sf::Vector2f>(1000, 500),
        sf::Color::Black
    );

    std::vector< std::shared_ptr<Line> > *lines = new std::vector< std::shared_ptr<Line> >();
    bool draw = true;

    lines->push_back(l);
    int i = 0;
    bool lastDraw = false;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        if (lines->size() > 0)
        {
            lines->front()->Draw(window);
            window->display();
            processLine(lines);
        } else if (lastDraw == false) {
            window->display();
            lastDraw = true;
        }
        
        i++;
    }

    return 0;
}