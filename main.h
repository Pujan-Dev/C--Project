// simple_menu.cpp

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class SimpleMenu
{
private:
    RenderWindow &window;
    Font font;
    Text titleText;
    Text startText;
    Text exitText;
    int selectedIndex;

    void handleInput()
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Down)
                {
                    selectedIndex = (selectedIndex + 1) % 2; // Toggle between 0 and 1
                }
                else if (event.key.code == Keyboard::Up)
                {
                    selectedIndex = (selectedIndex - 1 + 2) % 2; // Toggle between 0 and 1
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    if (selectedIndex == 0)
                    {
                        std::cout << "Starting Pacman Game..." << std::endl;
                        // Initialize and run PacmanGame here if needed
                        // PacmanGame pacmanGame(window);
                        // pacmanGame.run();
                    }
                    else if (selectedIndex == 1)
                    {
                        window.close(); // Exit the application
                    }
                }
            }
        }
    }

    void update()
    {
        startText.setFillColor(selectedIndex == 0 ? Color::Yellow : Color::White);
        exitText.setFillColor(selectedIndex == 1 ? Color::Yellow : Color::White);
    }

    void render()
    {
        window.clear();
        window.draw(titleText);
        window.draw(startText);
        window.draw(exitText);
        window.display();
    }

public:
    SimpleMenu(RenderWindow &win) : window(win), selectedIndex(0)
    {
        if (!font.loadFromFile("path/to/your/font.ttf")) // Replace with actual path
        {
            std::cerr << "Failed to load font!" << std::endl;
        }

        titleText.setFont(font);
        titleText.setString("Main Menu");
        titleText.setCharacterSize(36);
        titleText.setFillColor(Color::White);
        titleText.setPosition(200, 50);

        startText.setFont(font);
        startText.setString("Start Game");
        startText.setCharacterSize(24);
        startText.setFillColor(Color::White);
        startText.setPosition(300, 150);

        exitText.setFont(font);
        exitText.setString("Exit");
        exitText.setCharacterSize(24);
        exitText.setFillColor(Color::White);
        exitText.setPosition(300, 200);
    }

    void run()
    {
        while (window.isOpen())
        {
            handleInput();
            update();
            render();
        }
    }
};

// Entry point
int main()
{
    RenderWindow window(VideoMode(800, 600), "Simple Menu");

    SimpleMenu menu(window);
    menu.run();

    return 0;
}
