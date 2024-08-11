#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cstdlib>

#include "pacman/wallsandpowerups.cpp"
#include "pacman/enemy.cpp"
// #include "pacman/pacman.cpp"
#include "./pacman/WallGenerator.h" // Include the header for WallGenerator

bool GOING_TO_BE_KILLED = false;
bool CAN_KILL_ENEMY = false;
int POINTS = 0;
bool gameWon = false; // Flag to track if the game is won

class PacmanGame
{

private:
    Text winMessage; // New variable to display win message
    float currentScale = 1.0f;
    bool scalingUp = true;
    RenderWindow window;
    Sprite pacman;
    Vector2f velocity{3.0f, 3.0f};
    float currentFrame = 0.0f;
    float animationSpeed = 0.1f;
    std::vector<Texture> pacmanLeftTextures;
    std::vector<Texture> pacmanRightTextures;
    std::vector<Texture> pacmanUpTextures;
    std::vector<Texture> pacmanDownTextures;
    std::vector<Texture> ghostTextures;
    std::vector<Enemy> enemies;
    std::vector<PowerUp> apple;
    std::vector<Pickup> strawberry;
    WallGenerator wallGenerator; // Use WallGenerator for walls
    Font font;
    Text scoreText; // Decl      // Set the position on the screen

    bool powerUpActive = false;   // To check if the power-up is active
    float powerUpDuration = 5.0f; // Duration in seconds
    Clock powerUpClock;           // SFML clock to track time

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
            if (!texture.loadFromFile(path + std::to_string(i) + ".png"))
            {
                std::cerr << "Failed to load " << path + std::to_string(i) + ".png" << std::endl;
            }
            textures.push_back(texture);
        }
    }

    void loadGhostTextures()
    {
        ghostTextures.clear(); // Clear previous textures
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
                std::cerr << "Failed to load ghost texture: blue_ghost.png" << std::endl;
            }
            ghostTextures.push_back(texture);
        }
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

    Vector2f getRandomPosition(float radius)
    {
        Vector2f position;
        bool collision;

        do
        {
            // Generate a random position within the window bounds
            position.x = static_cast<float>(rand() % (800 - static_cast<int>(radius * 2)) + radius);
            position.y = static_cast<float>(rand() % (600 - static_cast<int>(radius * 2)) + radius);

            collision = false;

            // Check collision with walls
            if (wallGenerator.checkCollision(FloatRect(position.x - radius, position.y - radius, radius * 2, radius * 2)))
            {
                collision = true;
            }

            // Check collision with Pacman
            FloatRect pacmanBounds = pacman.getGlobalBounds();
            if (pacmanBounds.intersects(FloatRect(position.x - radius, position.y - radius, radius * 2, radius * 2)))
            {
                collision = true;
            }

            // Check collision with enemies
            for (const auto &enemy : enemies)
            {
                if (enemy.sprite.getGlobalBounds().intersects(FloatRect(position.x - radius, position.y - radius, radius * 2, radius * 2)))
                {
                    collision = true;
                    break;
                }
            }

        } while (collision); // Keep generating positions until a valid one is found

        return position;
    }
    void update()
    {
        static int moving_ = -1;          // -1 means no movement
        static bool isKeyReleased = true; // To track if the key was released

        // Backup Pacman's position before movement
        Vector2f oldPosition = pacman.getPosition();

        // Update Pacman's movement and animation when a key is pressed
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
        {
            pacman.move(-velocity.x, 0);
            updateAnimation(pacmanLeftTextures);
            moving_ = 0;
            isKeyReleased = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
        {
            pacman.move(velocity.x, 0);
            updateAnimation(pacmanRightTextures);
            moving_ = 1;
            isKeyReleased = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
        {
            pacman.move(0, -velocity.y);
            updateAnimation(pacmanUpTextures);
            moving_ = 2;
            isKeyReleased = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
        {
            pacman.move(0, velocity.y);
            updateAnimation(pacmanDownTextures);
            moving_ = 3;
            isKeyReleased = false;
        }
        else
        {
            isKeyReleased = true;
        }

        // Check for collisions and revert position if necessary
        if (wallGenerator.checkCollision(pacman.getGlobalBounds()))
        {
            pacman.setPosition(oldPosition);
        }

        // Continue moving automatically until a key is released
        if (isKeyReleased)
        {
            if (moving_ == 0)
            {
                pacman.move(-velocity.x, 0);
                updateAnimation(pacmanLeftTextures);
            }
            else if (moving_ == 1)
            {
                pacman.move(velocity.x, 0);
                updateAnimation(pacmanRightTextures);
            }
            else if (moving_ == 2)
            {
                pacman.move(0, -velocity.y);
                updateAnimation(pacmanUpTextures);
            }
            else if (moving_ == 3)
            {
                pacman.move(0, velocity.y);
                updateAnimation(pacmanDownTextures);
            }
        }
        if (wallGenerator.checkCollision(pacman.getGlobalBounds()))
        {
            pacman.setPosition(oldPosition);
        }

        // Check if any key was released
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased)
            {
                isKeyReleased = true;
            }
        }

        // Ensure Pacman stays within window bounds
        FloatRect pacmanBounds = pacman.getGlobalBounds();
        if (pacmanBounds.left < 0)
        {
            pacman.setPosition(0, pacmanBounds.top);
            moving_ = -1; // Stop movement if hit the left wall
        }
        if (pacmanBounds.top < 0)
        {
            pacman.setPosition(pacmanBounds.left, 0);
            moving_ = -1; // Stop movement if hit the top wall
        }
        if (pacmanBounds.left + pacmanBounds.width > 800)
        {
            pacman.setPosition(800 - pacmanBounds.width, pacmanBounds.top);
            moving_ = -1; // Stop movement if hit the right wall
        }
        if (pacmanBounds.top + pacmanBounds.height > 600)
        {
            pacman.setPosition(pacmanBounds.left, 600 - pacmanBounds.height);
            moving_ = -1; // Stop movement if hit the bottom wall
        }

        // Update enemies
        for (auto &enemy : enemies)
        {
            enemy.update();
            FloatRect enemyBounds = enemy.sprite.getGlobalBounds();
            if (pacmanBounds.intersects(enemyBounds) && CAN_KILL_ENEMY)
            {
                enemy.sprite.setPosition(400, 300); // Center of the window (800 x 600)
            }

            // Check for collisions between enemy and walls
            if (wallGenerator.checkCollision(enemy.sprite.getGlobalBounds()))
            {
                enemy.velocity.x = -enemy.velocity.x; // Revert direction
                enemy.velocity.y = -enemy.velocity.y; // Revert direction
            }
        }

        // Update power-ups
        // (Existing code)
        // Animate strawberries (pulsing effect)
        float scaleSpeed = 0.006f; // Speed of scaling
        float maxScale = 1.2f;     // Maximum scale
        float minScale = 1.0f;     // Minimum scale

        if (scalingUp)
        {
            currentScale += scaleSpeed;
            if (currentScale >= maxScale)
                scalingUp = false;
        }
        else
        {
            currentScale -= scaleSpeed;
            if (currentScale <= minScale)
                scalingUp = true;
        }

        for (auto &Pickup : strawberry)
        {
            Pickup.shape.setScale(currentScale, currentScale);
        }
        for (auto &Pickup : strawberry)
        {
            FloatRect strawberry_bounds = Pickup.shape.getGlobalBounds();
            if (pacmanBounds.intersects(strawberry_bounds))
            {
                POINTS += 1;
                Pickup.shape.setPosition(800, 800);
            }
        }
        scoreText.setString("Score: " + std::to_string(POINTS));

        // Update power-ups

        for (auto &powerup : apple)
        {
            FloatRect appleBounds = powerup.sprite.getGlobalBounds();
            if (pacmanBounds.intersects(appleBounds))
            {
                std::srand(std::time(0));

                int x, y;
                bool collision;

                do
                {
                    // Define the spawnable area within the blue walls
                    int leftMargin = 50;    // Left boundary (x = 50)
                    int rightMargin = 740;  // Right boundary (x + width = 740)
                    int topMargin = 50;     // Top boundary (y = 50)
                    int bottomMargin = 540; // Bottom boundary (y + height = 540)

                    // Adjust the margins considering the size of the apple
                    int maxX = rightMargin - static_cast<int>(appleBounds.width);
                    int maxY = bottomMargin - static_cast<int>(appleBounds.height);

                    // Generate random positions within the defined area
                    x = std::rand() % (maxX - leftMargin + 1) + leftMargin;
                    y = std::rand() % (maxY - topMargin + 1) + topMargin;

                    powerup.sprite.setPosition(x, y);

                    // Check if the apple collides with any walls
                    collision = wallGenerator.checkCollision(powerup.sprite.getGlobalBounds());

                } while (collision); // Repeat until no collision with walls

                CAN_KILL_ENEMY = true;
                powerUpActive = true;   // Activate the power-up
                powerUpClock.restart(); // Restart the power-up clock
                loadGhostTextures();    // Reload ghost textures when power-up is collected
            }
        }
        // Deactivate the power-up after the duration ends
        if (powerUpActive && powerUpClock.getElapsedTime().asSeconds() > powerUpDuration)
        {
            CAN_KILL_ENEMY = false;
            powerUpActive = false;
            loadGhostTextures(); // Reload ghost textures when power-up effect ends
        }
        scoreText.setString("Score: " + std::to_string(POINTS));
        // GAME win state

        // Draw win message if game is won
        if (POINTS == 6)
        {
            gameWon = true;
        }
    }

    void render()
    {
        if (!gameWon)
        {

            window.clear();
            window.draw(pacman);
            wallGenerator.draw(window); // Draw walls

            // Draw enemies
            for (const auto &enemy : enemies)
            {
                window.draw(enemy.sprite);
            }

            // Draw power-ups
            for (const auto &powerup : apple)
            {
                window.draw(powerup.sprite);
            }
            for (const auto &Pickup : strawberry)
            {
                window.draw(Pickup.shape);
            }
            window.draw(scoreText);
            window.display();
        }
        else
        {
            window.clear();
            window.draw(winMessage);
            window.display();
        }
    }

public:
    PacmanGame() : window(VideoMode(800, 600), "Pacman Game")
    {

        scoreText.setFont(font);
        scoreText.setString("Score: 0");      // Initial score text
        scoreText.setCharacterSize(24);       // Text size
        scoreText.setFillColor(Color::White); // Text color
        scoreText.setPosition(10, 10);        // Position on the screen
        window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));

        // Load font
        if (!font.loadFromFile("src/arial.ttf"))
        {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Load textures for Pacman's animations
        loadTextures(pacmanLeftTextures, "pacman-art/pacman-left/", 3);
        loadTextures(pacmanRightTextures, "pacman-art/pacman-right/", 3);
        loadTextures(pacmanUpTextures, "pacman-art/pacman-up/", 3);
        loadTextures(pacmanDownTextures, "pacman-art/pacman-down/", 3);

        // Load textures for enemies
        loadGhostTextures();

        // win state
        winMessage.setFont(font);
        winMessage.setString("You Win!");       // Message to display
        winMessage.setCharacterSize(48);        // Larger text size for visibility
        winMessage.setFillColor(Color::Yellow); // Color of the text
        winMessage.setPosition(250, 250);       // Position on the screen
        winMessage.setScale(1.5f, 1.5f);        // Scale the text if needed
        // Setup Pacman
        pacman.setTexture(pacmanRightTextures[0]);
        pacman.setPosition(400, 500);
        pacman.setScale(1.750f, 1.750f); // Scale Pacman to twice its original size

        // Setup enemies
        enemies.emplace_back(ghostTextures[0], Vector2f(150, 100), Vector2f(1, 1));
        enemies.emplace_back(ghostTextures[1], Vector2f(200, 200), Vector2f(-1, 1));
        enemies.emplace_back(ghostTextures[2], Vector2f(300, 300), Vector2f(1, -1));
        enemies.emplace_back(ghostTextures[3], Vector2f(400, 400), Vector2f(-1, -1));

        // Scale enemies
        for (auto &enemy : enemies)
        {
            enemy.sprite.setScale(1.2f, 1.2f); // Scale enemies to twice their original size
        }

        // Setup power-ups
        strawberry.emplace_back(Vector2f(90, 520));
        strawberry.emplace_back(Vector2f(720, 520));
        strawberry.emplace_back(Vector2f(720, 80));
        strawberry.emplace_back(Vector2f(720, 240));
        strawberry.emplace_back(Vector2f(90, 260));
        strawberry.emplace_back(Vector2f(90, 80));

        // Drawing strawberries (red circles)
        for (const auto &Pickup : strawberry)
        {
            window.draw(Pickup.shape);
        }

        // Initialize wall generator
        wallGenerator = WallGenerator();
    }

    void run()
    {
        while (window.isOpen()) // Loop runs until the window is closed or game is won
        {
            handleEvents();
            update();
            render();
        }
    }
};
