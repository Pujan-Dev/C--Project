#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>

// Include the game headers
#include "code/pacman.cpp" // Include the Pacman game logic
// #include "code/snake.cpp" // Include the Snake game logic
// #include "code/tetris.cpp" // Include the Tetris game logic
// #include "code/flappybird.cpp" // Include the Flappy Bird game logic

// Developer names and GitHub links
std::map<std::string, std::string> developersMap = {
    {"samridi ghimira", "https://github.com/developer3"},
    {"parsamsa adhakari", "https://github.com/developer4"},
    {"sujal karki", "https://github.com/developer2"},
    {"Pujan Neupane", "https://github.com/pujanneupane"}
};

enum class MenuState
{
    MAIN,
    DEVS
};

void openDeveloperLink(const std::string &url)
{
    std::string command;
#if defined(__linux__)
    command = "xdg-open " + url;
#elif defined(_WIN32)
    command = "start " + url;
#elif defined(__APPLE__)
    command = "open " + url;
#else
    std::cerr << "Unsupported OS" << std::endl;
    return;
#endif
    system(command.c_str());
}

void handleDeveloperClick(sf::Vector2i mousePos, const std::vector<sf::Text> &developerTexts)
{
    for (const auto &text : developerTexts)
    {
        if (text.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            auto it = developersMap.find(text.getString());
            if (it != developersMap.end())
            {
                openDeveloperLink(it->second);
                return;
            }
        }
    }
}

void animateTitleOnHover(sf::Text &title, sf::Clock &clock, sf::Vector2i mousePos, sf::RenderWindow &window, bool mouseHeld)
{
    float time = clock.getElapsedTime().asSeconds();
    if (!title.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        if (mouseHeld)
        {
            float tilt = std::sin(time * 8) * 1.0f;
            title.setRotation(tilt);
            title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);
        }
        else
        {
            float wobble = std::sin(time * 4) * 2.0f;
            title.setRotation(0);
            title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50 + wobble);
        }
    }
    else
    {
        title.setRotation(0);
        title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);
    }
}

void handleButtonHover(sf::Text &button, sf::Vector2i mousePos)
{
    if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
    {
        button.setFillColor(sf::Color(72, 128, 250));
    }
    else
    {
        button.setFillColor(sf::Color::White);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Classic Games Menu");

    sf::Font font;
    if (!font.loadFromFile("src/SuperPixel.ttf"))
    {
        std::cerr << "Could not load font!" << std::endl;
        return -1;
    }

    sf::Text title("Classic Games", font, 40);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 50);

    std::vector<sf::Text> menu;
    std::vector<std::string> menuOptions = {"Pacman", "Snake", "Tetris", "Flappy Bird", "Quit"};

    for (size_t i = 0; i < menuOptions.size(); ++i)
    {
        sf::Text item;
        item.setFont(font);
        item.setString(menuOptions[i]);
        item.setCharacterSize(25);
        item.setFillColor(sf::Color::White);
        item.setPosition(window.getSize().x / 2 - item.getGlobalBounds().width / 2, 200 + i * 60);
        menu.push_back(item);
    }

    sf::Text devsButton("Devs", font, 20);
    devsButton.setFillColor(sf::Color(255, 165, 0));
    devsButton.setPosition(50, 500);

    sf::Text backButton("Back", font, 30);
    backButton.setPosition(50, 500);

    sf::Text devs("Developers", font, 30);
    devs.setPosition(window.getSize().x / 2 - devs.getGlobalBounds().width / 2, 50);

    std::vector<sf::Text> developerTexts;
    float yOffset = 200;
    for (const auto &pair : developersMap)
    {
        sf::Text devText(pair.first, font, 20);
        devText.setPosition(window.getSize().x / 2 - devText.getGlobalBounds().width / 2, yOffset);
        developerTexts.push_back(devText);
        yOffset += 40; // Adjust spacing between names
    }

    MenuState menuState = MenuState::MAIN;
    sf::Clock clock;
    bool mouseHeld = false;

    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseHeld = true;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                mouseHeld = false;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (menuState == MenuState::MAIN)
                {
                    for (size_t i = 0; i < menu.size(); ++i)
                    {
                        if (menu[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
                        {
                            if (i == 0)
                            {
                                window.close();
                                PacmanGame game;
                                game.run();
                            }
                            else if (i == 4)
                            {
                                window.close();
                            }
                        }
                    }

                    if (devsButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuState = MenuState::DEVS;
                    }
                }
                else if (menuState == MenuState::DEVS)
                {
                    if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        menuState = MenuState::MAIN;
                    }
                    else
                    {
                        handleDeveloperClick(mousePos, developerTexts);
                    }
                }
            }
        }

        window.clear();

        if (menuState == MenuState::MAIN)
        {
            animateTitleOnHover(title, clock, mousePos, window, mouseHeld);

            window.draw(title);

            for (auto &item : menu)
            {
                handleButtonHover(item, mousePos);
                window.draw(item);
            }

            handleButtonHover(devsButton, mousePos);
            window.draw(devsButton);
        }
        else if (menuState == MenuState::DEVS)
        {
            devs.setFillColor(sf::Color::Yellow);
            backButton.setFillColor(sf::Color::Red);
            animateTitleOnHover(devs, clock, mousePos, window, mouseHeld);

            window.draw(devs);
            for (const auto &text : developerTexts)
            {
                window.draw(text);
            }
            window.draw(backButton);
        }

        window.display();
    }

    return 0;
}
