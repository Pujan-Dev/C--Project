#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player and Walls");

    // Create player character (circle)
    sf::CircleShape player(20.0f); // Radius 20
    player.setFillColor(sf::Color::Green);
    player.setPosition(100.0f, 100.0f); // Starting position

    // Create walls
    std::vector<sf::RectangleShape> walls;

    // Example wall
    sf::RectangleShape wall1(sf::Vector2f(200.0f, 20.0f));
    wall1.setFillColor(sf::Color::Red);
    wall1.setPosition(300.0f, 150.0f);

    // Add the wall to the vector
    walls.push_back(wall1);
    // Movement speed
    float speed = 0.50f;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Movement
        sf::Vector2f movement(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += speed;

        // Apply movement
        player.move(movement);

        // Check for collisions with walls
        for (auto& wall : walls) {
            if (player.getGlobalBounds().intersects(wall.getGlobalBounds())) {
                // Reverse movement if collision occurs
                player.move(-movement);
            }
        }

        // Clear window
        window.clear();

        // Draw player and walls
        window.draw(player);
        for (auto& wall : walls) {
            window.draw(wall);
        }

        // Display everything
        window.display();
    }

    return 0;
}
