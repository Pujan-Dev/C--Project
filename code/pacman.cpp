#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <SFML/Audio.hpp>
#include <fstream>

#include "pacman/wallsandpowerups.cpp"
#include "pacman/enemy.cpp"
#include "./pacman/WallGenerator.h"
class Button
{
public:
    static bool renderQuitButton(RenderWindow &window, Font &font)
    {
        // Define button properties
        Text text;
        RectangleShape buttonShape;
        Color idleColor = Color(255, 255, 102);
        Color hoverColor = Color(255, 255, 51);
        Color activeColor = Color(255, 215, 0);

        // Setup font
        text.setFont(font);
        text.setString("Quit");
        text.setCharacterSize(20);
        text.setFillColor(Color::Black);

        // Setup button shape
        buttonShape.setSize(Vector2f(200, 50));
        buttonShape.setPosition(300, 500);
        buttonShape.setFillColor(idleColor);

        text.setPosition(300 + (200 / 2.f) - (text.getGlobalBounds().width / 2.f), 500 + (50 / 2.f) - (text.getGlobalBounds().height / 2.f));

        window.draw(buttonShape);
        window.draw(text);

        // Check if mouse is hovering over the button
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        if (buttonShape.getGlobalBounds().contains(mousePos))
        {
            buttonShape.setFillColor(hoverColor);
        }
        else
        {
            buttonShape.setFillColor(idleColor);
        }
        window.draw(buttonShape);
        window.draw(text);

        // Handle button press
        if (Mouse::isButtonPressed(Mouse::Left) && buttonShape.getGlobalBounds().contains(mousePos))
        {
            window.close();
        }
        return false; // Otherwise, continue the game
    }
};

class PacmanGame
{

private:
    bool GOING_TO_BE_KILLED = false;
    bool CAN_KILL_ENEMY = false;
    int TOTAL_LIVES = 3;
    int POINTS = 0;
    bool gameWon = false; // Flag to track if the game is won
    Text winMessage;      // New variable to display win message
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
    Time elapsedTime; // Time elapsed since the game started
    Clock timerClock; // SFML clock to track the elapsed time
    Text timerText;   // Text to display the timer

    Sound_to s;
    WallGenerator wallGenerator; // Use WallGenerator for walls
    Font font;
    Text scoreText; // Decl      // Set the position on the screen

    Text livesText;    // Text to display the number of lives
    Text HIGHEST_TIME; // Text to display the highest time

    bool powerUpActive = false;   // To check if the power-up is active
    float powerUpDuration = 5.0f; // Duration in seconds
    Clock powerUpClock;           // SFML clock to track time
    Text yourtime;
    // implement file handling in c++
    void
    get_text_from_file(Text &highestText)
    {
        std::ifstream file("highest_time.txt");
        if (file.is_open())
        {
            std::string line;
            if (std::getline(file, line))
            {
                highestText.setString(line);
            }
            else
            {
                highestText.setString("Peak Time: 0:00"); // Default value if file is empty
            }
            file.close();
        }
        else
        {
            std::cerr << "Failed to open file!" << std::endl;
        }
    }

    void save_text_to_file(const Text &currentText)
    {
        std::string newHighScore = currentText.getString();

        std::ifstream file("highest_time.txt");
        std::string currentHighScore = "Peak Time: 0:00";
        if (file.is_open())
        {
            std::getline(file, currentHighScore);
            file.close();
        }

        if (currentHighScore == "Peak Time: 0:00" || newHighScore < currentHighScore)
        {
            std::ofstream outFile("highest_time.txt");
            if (outFile.is_open())
            {
                outFile << newHighScore;
                outFile.close();
            }
            else
            {
                std::cerr << "Failed to open file for writing!" << std::endl;
            }
        }
    }

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
        elapsedTime = timerClock.getElapsedTime();
        int seconds = static_cast<int>(elapsedTime.asSeconds());
        int minutes = seconds / 60;
        seconds %= 60;
        timerText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));

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

            // is the bellow code correct? copilot says it is not correct then what is the correct code?
            if (enemyBounds.intersects(pacmanBounds) && !CAN_KILL_ENEMY && !GOING_TO_BE_KILLED) // Pacman is killed
            {
                pacman.setPosition(400, 500);
                TOTAL_LIVES -= 1;
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
            FloatRect strawberryBounds = Pickup.shape.getGlobalBounds();
            if (pacmanBounds.intersects(strawberryBounds))
            {
                POINTS += 1;
                yourtime = timerText;

                Pickup.shape.setPosition(800, 800);
                std::srand(std::time(0));

                CAN_KILL_ENEMY = true;
                powerUpActive = true; // Activate the power-up
                powerUpClock.restart();
                s.playSound("code/Sounds/eaten.wav");
            }
        }
        scoreText.setString("Score: " + std::to_string(POINTS));
        livesText.setString("Lives: " + std::to_string(TOTAL_LIVES));

        // Update power-ups

        for (auto &powerup : apple)
        {
            FloatRect appleBounds = powerup.sprite.getGlobalBounds();
            if (pacmanBounds.intersects(appleBounds))
            {
                std::srand(std::time(0));
                yourtime = timerText;

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

        // GAME win state

        // Draw win message if game is won
        if (POINTS == 6) // Ensure sound plays only once
        {
            gameWon = true;
        }

        if (gameWon)
        {

            save_text_to_file(timerText);
        }
    }

    void render()
    {

        if (!gameWon && TOTAL_LIVES > 0)
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

            // Draw timer
            window.draw(timerText);

            // Draw score
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(HIGHEST_TIME);

            window.display();
        }
        else if (TOTAL_LIVES == 0)
        {
            window.clear();
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(48);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(200, 250);
            gameOverText.setScale(1.5f, 1.5f);
            window.draw(gameOverText);
            // stop all the sound
            s.background_nosound();


            Button::renderQuitButton(window, font);
            window.display();
        }
        else
        {
            window.clear();
            Button::renderQuitButton(window, font);

            window.draw(winMessage);
            window.draw(HIGHEST_TIME);
            window.draw(yourtime);
            window.display();
        }
    }

    void setupTextProperties()
    {
        timerText.setFont(font);
        timerText.setCharacterSize(20);
        timerText.setFillColor(Color::White);
        timerText.setPosition(350, 10);

        livesText.setFont(font);
        livesText.setString("Lives: 3");
        livesText.setCharacterSize(20);
        livesText.setFillColor(Color::White);
        livesText.setPosition(10, 10);

        HIGHEST_TIME.setFont(font);
        HIGHEST_TIME.setString("Peak Time: 0:00");
        HIGHEST_TIME.setCharacterSize(20);
        HIGHEST_TIME.setFillColor(Color::White);
        HIGHEST_TIME.setPosition(600, 10);

        winMessage.setFont(font);
        winMessage.setString("You Win!");
        winMessage.setCharacterSize(48);
        winMessage.setFillColor(Color::Yellow);
        winMessage.setPosition(250, 250);
        winMessage.setScale(1.5f, 1.5f);

        scoreText.setFont(font);
        scoreText.setString("Score: 0");
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(350, 570);
        yourtime.setFont(font);
        yourtime.setCharacterSize(20);
        yourtime.setFillColor(Color::White);
        yourtime.setPosition(350, 570);
    }

public:
    // pacman takes win as an arg
    PacmanGame() : window(VideoMode(800, 600), "Pacman Game"), GOING_TO_BE_KILLED(false), CAN_KILL_ENEMY(false), TOTAL_LIVES(3), POINTS(0), gameWon(false)
    {
        s.background_song();
        timerClock.restart(); // Start the clock

        setupTextProperties();

        window.setFramerateLimit(60);
        srand(static_cast<unsigned>(time(nullptr)));

        // Load font
        if (!font.loadFromFile("pacman-art/Joystix.TTF"))
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
        try
        {
            get_text_from_file(HIGHEST_TIME);
            throw std::runtime_error("Error: File not found");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "Error: Unknown exception" << '\n';
        }
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
