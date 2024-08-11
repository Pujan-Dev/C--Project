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

        // Clear any previous walls
        walls.clear();

        // Outer boundary walls
        walls.push_back(createWall(50, 50, 700, 10));  // Top horizontal
        walls.push_back(createWall(50, 550, 710, 10)); // Bottom horizontal
        walls.push_back(createWall(50, 50, 10, 500));  // Left vertical
        walls.push_back(createWall(750, 50, 10, 500)); // Right vertical

        // // left part of the level
        walls.push_back(createWall(100, 50, 15, 150));
        walls.push_back(createWall(100, 450, 15, 100));
        walls.push_back(createWall(50, 250, 100, 15));
        walls.push_back(createWall(50, 375, 100, 15));

        // // Central horizontal walls
        walls.push_back(createWall(250, 150, 350, 15));
        walls.push_back(createWall(250, 450, 350, 15));

        // // Middle structure around the ghost house
        // walls.push_back(createWall(325, 250, 150, 10)); // Top horizontal
        walls.push_back(createWall(325, 320, 155, 15)); // Bottom horizontal
        walls.push_back(createWall(325, 250, 15, 80));  // Left vertical
        walls.push_back(createWall(465, 245, 15, 80));  // Right vertical

        // // right part of the level
        walls.push_back(createWall(700, 50, 15, 150));
        walls.push_back(createWall(700, 450, 15, 100));
        walls.push_back(createWall(650, 250, 100, 15));
        walls.push_back(createWall(650, 375, 100, 15));

        // minnor blocks
        walls.push_back(createWall(130, 300, 15, 45));
        walls.push_back(createWall(650, 300, 15, 45));

        // center walls
        walls.push_back(createWall(450, 490, 15, 35));
        walls.push_back(createWall(350, 490, 15, 35));


        // // Tunnels (openings on left and right)
        // walls.push_back(createWall(10, 300, 60, 10));  // Left tunnel
        // walls.push_back(createWall(730, 300, 60, 10)); // Right tunnel

        // // Additional walls to create pathways (mirrored on both sides)
        // walls.push_back(createWall(150, 150, 150, 10)); // Upper left horizontal
        // walls.push_back(createWall(500, 150, 150, 10)); // Upper right horizontal
        // walls.push_back(createWall(150, 450, 150, 10)); // Lower left horizontal
        // walls.push_back(createWall(500, 450, 150, 10)); // Lower right horizontal

        // walls.push_back(createWall(150, 150, 10, 100)); // Upper left vertical
        // walls.push_back(createWall(650, 150, 10, 100)); // Upper right vertical
        // walls.push_back(createWall(150, 350, 10, 100)); // Lower left vertical
        // walls.push_back(createWall(650, 350, 10, 100)); // Lower right vertical

        // // Central horizontal lines
        // walls.push_back(createWall(250, 300, 300, 10)); // Middle horizontal line

        // Adding more walls as needed to match the exact layout
        // Ensure that the left side mirrors the right side
        // Add more walls as per the structure in the image

        // You can continue to add more walls to match the design from the image
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

#endif
