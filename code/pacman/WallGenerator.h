#ifndef WALLGENERATOR_H
#define WALLGENERATOR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

using namespace sf;

class WallGenerator
{
private:
    std::vector<RectangleShape> walls;

public:
    WallGenerator()
    {
        generateWalls();
    }

    void generateWalls()
    {
        walls.clear();

        // Define walls with hardcoded positions and sizes
        // Format: x y width height
        addWall(00, 100, 20, 20);   // Example wall
        // Add more walls as needed
    }

    void addWall(float x, float y, float width, float height)
    {
        RectangleShape wall(Vector2f(width, height));
        wall.setFillColor(Color::Red);
        wall.setPosition(x, y);
        walls.push_back(wall);
    }

    void draw(RenderWindow &window)
    {
        for (const auto &wall : walls)
        {
            window.draw(wall);
        }
    }

    bool checkCollision(const FloatRect &bounds) const
    {
        for (const auto &wall : walls)
        {
            if (bounds.intersects(wall.getGlobalBounds()))
            {
                return true;
            }
        }
        return false;
    }
};

#endif // WALLGENERATOR_H
