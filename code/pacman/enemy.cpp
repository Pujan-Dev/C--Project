// #include "global.cpp"

class Enemy
{
public:
    Sprite sprite;
    Vector2f velocity;

    Enemy(Texture &texture, Vector2f startPosition, Vector2f startVelocity)
    {
        // if (GOING_TO_BE_KILLED == false)
        // {
            sprite.setTexture(texture);
        // }/
        // else
        // {/
            // Texture texture;
            // if (!texture.loadFromFile("pacman-art/ghosts/blue_ghost.png"))
            // {
                // std::cerr << "Failed to load ghost texture: .png" << std::endl;
            // }
            // sprite.setTexture("../pacman-art/ghosts/blue_ghost.png")
        // }
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