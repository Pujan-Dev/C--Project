#ifndef WALLGENERATOR_H
#define WALLGENERATOR_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;

class WallGenerator
{
private:
    std::vector<RectangleShape> walls;

public:
    WallGenerator()
    {
        generateComplexWalls();
    }

    void generateComplexWalls()
    {
        // Clear any previous walls
        walls.clear();

        // Outer boundary walls
        walls.push_back(createWall(50, 50, 700, 20));  // Top horizontal
        walls.push_back(createWall(50, 550, 720, 20)); // Bottom horizontal
        walls.push_back(createWall(50, 50, 20, 500));  // Left vertical
        walls.push_back(createWall(750, 50, 20, 500)); // Right vertical

        // Middle part
        walls.push_back(createWall(380, 250, 20, 120)); // Left vertical
        walls.push_back(createWall(500, 250, 20, 120)); // Right vertical
        walls.push_back(createWall(380, 350, 140, 20)); // Horizontal

        // Left wall
        walls.push_back(createWall(120, 60, 20, 100));
        walls.push_back(createWall(120, 220, 20, 120));
        walls.push_back(createWall(60, 220, 60, 20));
        walls.push_back(createWall(60, 400, 80, 20));
        walls.push_back(createWall(120, 480, 20, 80));

        // Central walls
        walls.push_back(createWall(350, 150, 170, 20));
        walls.push_back(createWall(350, 450, 170, 20));

        // Right walls
        walls.push_back(createWall(680, 200, 80, 20));
        walls.push_back(createWall(680, 400, 80, 20));
        walls.push_back(createWall(680, 220, 20, 80));
        walls.push_back(createWall(680, 60, 20, 80));
        walls.push_back(createWall(680, 490, 20, 80));

        // Obstacles
        walls.push_back(createWall(220, 450, 20, 100));
        walls.push_back(createWall(220, 60, 20, 120));

        // C/G in left
        walls.push_back(createWall(200, 250, 20, 100));
        walls.push_back(createWall(200, 250, 100, 20));
        walls.push_back(createWall(200, 350, 100, 20));
        walls.push_back(createWall(600, 200, 20, 250));
    }

    RectangleShape createWall(float x, float y, float width, float height)
    {
        Color lightBlue(62, 133, 247);

        RectangleShape wall(Vector2f(width, height));
        wall.setPosition(x, y);
        wall.setFillColor(lightBlue); // Set wall color
        return wall;
    }

    bool checkCollision(const FloatRect &boundingBox) const
    {
        for (const auto &wall : walls)
        {
            if (wall.getGlobalBounds().intersects(boundingBox))
            {
                return true;
            }
        }
        return false;
    }

    void draw(RenderWindow &window)
    {
        for (const auto &wall : walls)
        {
            window.draw(wall);
        }
    }
};

#endif // WALLGENERATOR_H
