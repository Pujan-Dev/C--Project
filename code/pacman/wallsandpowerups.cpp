#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace sf;

class PowerUp
{
public:
    Sprite sprite;
    Texture texture; // Store the texture as a member

    PowerUp(Vector2f startPosition)
    {
        if (!texture.loadFromFile("pacman-art/other/apple.png"))
        {
            std::cerr << "Failed to load apple texture" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(startPosition);
    }
};
class Wall
{
public:
    std::vector<RectangleShape> walls;

    Wall()
    {
        // Add multiple walls
        RectangleShape wall1(Vector2f(200.0f, 20.0f));
        wall1.setFillColor(Color::Red);
        wall1.setPosition(300.0f, 150.0f);
        walls.push_back(wall1);

        RectangleShape wall2(Vector2f(150.0f, 20.0f));
        wall2.setFillColor(Color::Red);
        wall2.setPosition(100.0f, 100.0f);
        walls.push_back(wall2);

        // Add more walls as needed
    }

    void draw(RenderWindow &window)
    {
        for (auto &wall : walls)
        {
            window.draw(wall);
        }
    }

    bool checkCollision(const FloatRect &bounds)
    {
        for (auto &wall : walls)
        {
            if (wall.getGlobalBounds().intersects(bounds))
            {
                return true;
            }
        }
        return false;
    }
};