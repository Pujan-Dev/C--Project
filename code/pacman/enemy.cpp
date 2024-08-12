// #include "global.cpp"
#include <SFML/Graphics.hpp>
using namespace sf;
class Enemy
{
public:
    Sprite sprite;
    Vector2f velocity;

    Enemy(Texture &texture, Vector2f startPosition, Vector2f startVelocity)
    {
        sprite.setTexture(texture);
        sprite.setPosition(startPosition);
        velocity = startVelocity;
    }

    void update()
    {
        // Move randomly
        if (rand() % 100 < 2) // 2% chance to change direction
        {
            velocity.x = (rand() % 3 - 1) * 1.0f; // Random x direction (-1, 0, 1)
            velocity.y = (rand() % 3 - 1) * 1.0f; // Random y direction (-1, 0, 1)
        }
        sprite.move(velocity);

        // Keep enemies within the window bounds
        FloatRect bounds = sprite.getGlobalBounds();
        if (bounds.left < 0 || bounds.left + bounds.width > 800)
            velocity.x = -velocity.x;
        if (bounds.top < 0 || bounds.top + bounds.height > 600)
            velocity.y = -velocity.y;
    }
};