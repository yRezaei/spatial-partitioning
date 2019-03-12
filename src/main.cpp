#include <SFML/Graphics.hpp>
#include "../include/quadtree.hpp"
#include <ctime>

#define SEARCH_WIDTH 30
#define RAND_RANGE 100

void draw_quadtree_devissions(sf::RenderWindow& window, Quadtree& qtree)
{
    sf::Vertex line[2] = {sf::Vertex(sf::Vector2f(0.0f,0.0f)), sf::Vertex(sf::Vector2f(1.0f,1.0f))};
    if(qtree.is_devided()) 
    {
        auto& bound = qtree.boundery();
        line[0] = sf::Vertex(sf::Vector2f(bound.cx - bound.hw, bound.cy));
        line[1] = sf::Vertex(sf::Vector2f(bound.cx + bound.hw, bound.cy));
        window.draw(line, 2, sf::Lines);
        line[0] = sf::Vertex(sf::Vector2f(bound.cx, bound.cy - bound.hh));
        line[1] = sf::Vertex(sf::Vector2f(bound.cx, bound.cy + bound.hh));
        window.draw(line, 2, sf::Lines);
        for(auto q : qtree.sub_quads())
            draw_quadtree_devissions(window, *q);
    }
}


void draw_rect(sf::RenderWindow& window, const sf::Vector2f& p)
{
    sf::Vertex line[5] = {sf::Vertex(sf::Vector2f(p.x - SEARCH_WIDTH, p.y - SEARCH_WIDTH)), sf::Vertex(sf::Vector2f(p.x + SEARCH_WIDTH, p.y - SEARCH_WIDTH)), 
                                    sf::Vertex(sf::Vector2f(p.x + SEARCH_WIDTH, p.y + SEARCH_WIDTH)), sf::Vertex(sf::Vector2f(p.x - SEARCH_WIDTH, p.y + SEARCH_WIDTH)), sf::Vertex(sf::Vector2f(p.x - SEARCH_WIDTH, p.y - SEARCH_WIDTH)) };
    window.draw(line, 5, sf::LinesStrip);
}

float random( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

int main()
{
    srand((unsigned int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 800), "Spatial partitioning");
    window.setVerticalSyncEnabled(true);

    Quadtree qtree(AABB(400,400,400,400));

    bool mouse_left_is_down = false;
    bool mouse_right_is_down = false;
    bool mouse_is_moving = false;
    sf::Vector2f mouse_pos;
    std::vector<sf::CircleShape> particles;
    
    sf::Font font_arial;
    if (!font_arial.loadFromFile("arial.ttf"))
    {
        // error...
    }

    sf::Text txt_point_count;
    txt_point_count.setFont(font_arial);
    txt_point_count.setFillColor(sf::Color::Red);
    txt_point_count.setCharacterSize(24);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            mouse_is_moving = false;

            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        mouse_left_is_down = true;
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        mouse_right_is_down = true;
                    }
                    break;
                }
                case sf::Event::MouseButtonReleased:
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        mouse_left_is_down = false;
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        mouse_right_is_down = false;
                    }
                    break;
                }
                case sf::Event::MouseMoved:
                {
                    mouse_is_moving = true;
                    mouse_pos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

                    if(mouse_left_is_down) {
                        for(auto i = 0; i < RAND_RANGE; ++i) {
                            auto p = Point2D(random(event.mouseMove.x - RAND_RANGE, event.mouseMove.x + RAND_RANGE), random(event.mouseMove.y - RAND_RANGE, event.mouseMove.y + RAND_RANGE));
                            if(qtree.insert(p)) {
                                particles.emplace_back(sf::CircleShape(1.0f));
                                particles.back().setFillColor(sf::Color::Green);
                                particles.back().setPosition(p.x, p.y);
                            }
                        }
                        txt_point_count.setString("Particles Nr.: " + std::to_string(particles.size()));
                    }
                    break;
                }
            
                default:
                    break;
            }
        }

        window.clear();

        //draw_quadtree_devissions(window, qtree);

         for(auto& p : particles)
            window.draw(p);

        {
            if(mouse_right_is_down && mouse_is_moving) 
            {
                draw_rect(window, mouse_pos);

                std::vector<Point2D*> point_in_range;
                qtree.query(AABB(mouse_pos.x, mouse_pos.y, SEARCH_WIDTH, SEARCH_WIDTH), point_in_range);
                for(auto p : point_in_range) 
                {
                    auto pp = sf::CircleShape(2.0f);
                    pp.setPosition(p->x, p->y);
                    pp.setFillColor(sf::Color::Red);
                    window.draw(pp);
                }
            }
        }

        
        window.draw(txt_point_count);

        window.display();
    }

    return 0;
}