#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace sf;

const int TILE_SIZE = 32; // Adjust based on your wall tile size

bool CAN_KILL_ENEMY = false;
bool GOING_TO_BE_KILLED = false;

class Powerup
{
public:
    Sprite sprite;

    Powerup(Vector2f startPosition)
    {
        Texture texture;
        if (!texture.loadFromFile("pacman-art/other/strawberry.png"))
        {
            std::cerr << "Failed to load power-up texture: strawberry.png" << std::endl;
            return; // Exit constructor if texture fails to load
        }
        sprite.setTexture(texture);
        sprite.setPosition(startPosition);
        sprite.setScale(0.5f, 0.5f); // Adjust scale as needed
    }
};

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
        if (rand() % 100 < 2) // 2% chance to change direction
        {
            velocity.x = (rand() % 3 - 1) * 1.0f; // Random x direction (-5, 0, 5)
            velocity.y = (rand() % 3 - 1) * 1.0f; // Random y direction (-5, 0, 5)
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

class PacmanGame
{
private:
    RenderWindow window;
    Font font;
    Sprite pacman;
    Vector2f velocity{1.0f, 1.0f};
    float currentFrame = 0.0f;
    float animationSpeed = 0.1f;
    std::vector<Texture> pacmanLeftTextures;
    std::vector<Texture> pacmanRightTextures;
    std::vector<Texture> pacmanUpTextures;
    std::vector<Texture> pacmanDownTextures;
    std::vector<Texture> ghostTextures;
    std::vector<Enemy> enemies;
    std::vector<Powerup> apple;
    std::vector<RectangleShape> walls;       // Wall sprites
    std::vector<std::vector<bool>> wallGrid; // Grid to track walls

    void handleEvents()
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
    }
    void loadTextures(std::vector<Texture> &textures, const std::string &path, int count)
    {
        for (int i = 1; i <= count; ++i)
        {
            Texture texture;
            std::string filePath = path + std::to_string(i) + ".png";
            if (!texture.loadFromFile(filePath))
            {
                std::cerr << "Failed to load " << filePath << std::endl;
            }
            else
            {
                std::cout << "Loaded " << filePath << std::endl;
                textures.push_back(texture);
            }
        }
    }

    void loadGhostTextures()
    {
        if (!GOING_TO_BE_KILLED)
        {
            std::vector<std::string> ghostNames = {"blinky", "clyde", "inky", "pinky"};
            for (const auto &name : ghostNames)
            {
                Texture texture;
                if (!texture.loadFromFile("pacman-art/ghosts/" + name + ".png"))
                {
                    std::cerr << "Failed to load ghost texture: " << name << ".png" << std::endl;
                }
                ghostTextures.push_back(texture);
            }
        }
        else
        {
            Texture texture;
            if (!texture.loadFromFile("pacman-art/ghosts/blue_ghost.png"))
            {
                std::cerr << "Failed to load ghost texture: blue.png" << std::endl;
            }
            ghostTextures.push_back(texture);
        }
    }

    void loadWalls()
    {
        walls.clear(); // Clear previous walls if any

        // Create a blue rectangle for walls
        RectangleShape wallShape(Vector2f(TILE_SIZE, TILE_SIZE));
        wallShape.setFillColor(Color::Blue); // Set the color to blue

        // Example: Add some walls to the grid

        wallShape.setPosition(20 * TILE_SIZE, 10 * TILE_SIZE);
        walls.push_back(wallShape);
    }
    void updateAnimation(std::vector<Texture> &textures)
    {
        currentFrame += animationSpeed;
        if (currentFrame >= textures.size())
        {
            currentFrame = 0;
        }
        pacman.setTexture(textures[static_cast<int>(currentFrame)]);
    }

    Clock powerupClock;
    bool powerupActive = false;
    const float POWERUP_DURATION = 5.0f; // 5 seconds
    void update()
    {
 Vector2f movement(0.0f, 0.0f);

        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
        {
            movement.x = -velocity.x;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
        {
            movement.x = velocity.x;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
        {
            movement.y = -velocity.y;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
        {
            movement.y = velocity.y;
        }

        if (movement != Vector2f(0.0f, 0.0f))
        {
            pacman.move(movement);
            updateAnimation(pacmanRightTextures); // Adjust based on direction
        }
        // Calculate the next position
        Vector2f nextPosition = pacman.getPosition() + movement;

        // Check collision with walls
        bool collision = false;
        int gridX = static_cast<int>(nextPosition.x / TILE_SIZE);
        int gridY = static_cast<int>(nextPosition.y / TILE_SIZE);

        if (gridX >= 0 && gridY >= 0 && gridY < wallGrid.size() && gridX < wallGrid[0].size())
        {
            collision = wallGrid[gridY][gridX];
        }

        if (!collision)
        {
            pacman.move(movement);
            updateAnimation(pacmanRightTextures); // Adjust based on direction
        }

        // Ensure Pac-Man stays within window bounds
        FloatRect pacmanBounds = pacman.getGlobalBounds();
        if (pacmanBounds.left < 0)
            pacman.setPosition(0, pacmanBounds.top);
        if (pacmanBounds.top < 0)
            pacman.setPosition(pacmanBounds.left, 0);
        if (pacmanBounds.left + pacmanBounds.width > 800)
            pacman.setPosition(800 - pacmanBounds.width, pacmanBounds.top);
        if (pacmanBounds.top + pacmanBounds.height > 600)
            pacman.setPosition(pacmanBounds.left, 600 - pacmanBounds.height);

        for (auto &enemy : enemies)
        {
            FloatRect enemyBounds = enemy.sprite.getGlobalBounds();
            if (pacmanBounds.intersects(enemyBounds) && powerupActive)
            {
                enemy.sprite.setPosition(400, 300); // Center of the window (800 x 600)
            }
            enemy.update();
        }

        for (auto &powerups : apple)
        {
            FloatRect applebox = powerups.sprite.getGlobalBounds();
            if (pacmanBounds.intersects(applebox))
            {
                powerups.sprite.setPosition(40, 300); // Example reposition
                powerupActive = true;
                powerupClock.restart(); // Reset the power-up timer
            }
        }

        if (powerupActive && powerupClock.getElapsedTime().asSeconds() > POWERUP_DURATION)
        {
            powerupActive = false;
        }
    }
    void render()
    {
        window.clear();
        window.draw(pacman);

        for (const auto &enemy : enemies)
        {
            window.draw(enemy.sprite);
        }

        for (const auto &powerup : apple)
        {
            window.draw(powerup.sprite);
        }

        // Draw walls as rectangles
        for (const auto &wall : walls)
        {
            window.draw(wall);
        }

        window.display();
    }

public:
    PacmanGame() : window(VideoMode(800, 600), "Pacman Game")
    {
        window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));

        if (!font.loadFromFile("src/arial.ttf"))
        {
            std::cerr << "Failed to load font!" << std::endl;
        }

        loadTextures(pacmanLeftTextures, "pacman-art/pacman-left/", 3);
        loadTextures(pacmanRightTextures, "pacman-art/pacman-right/", 3);
        loadTextures(pacmanUpTextures, "pacman-art/pacman-up/", 3);
        loadTextures(pacmanDownTextures, "pacman-art/pacman-down/", 3);

        loadGhostTextures();
        loadWalls();

        pacman.setTexture(pacmanRightTextures[0]);
        pacman.setPosition(400, 300);

        enemies.emplace_back(ghostTextures[0], Vector2f(100, 100), Vector2f(1, 1));
        enemies.emplace_back(ghostTextures[1], Vector2f(200, 200), Vector2f(-1, 1));
        enemies.emplace_back(ghostTextures[2], Vector2f(300, 300), Vector2f(1, -1));
        enemies.emplace_back(ghostTextures[3], Vector2f(400, 400), Vector2f(-1, -1));
        apple.emplace_back(Vector2f(200, 300));
    }

    void run()
    {
        while (window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }
};
