#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class PacmanGame {
public:
    PacmanGame() : window(VideoMode(800, 600), "Pacman Game") {
        window.setFramerateLimit(60);

        // Load font
        if (!font.loadFromFile("src/arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        // Setup Pacman
        pacman.setRadius(20.f);
        pacman.setFillColor(Color::Yellow);
        pacman.setPosition(400, 300);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    RenderWindow window;
    Font font;
    CircleShape pacman;
    Vector2f velocity{5.0f, 0.0f};

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
    }

    void update() {
        // Basic movement for Pacman (left and right)
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            pacman.move(-velocity.x, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            pacman.move(velocity.x, 0);
        }
    }

    void render() {
        window.clear(Color::Black);
        window.draw(pacman);
        window.display();
    }
};
