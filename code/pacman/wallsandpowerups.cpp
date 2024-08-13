#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>

using namespace sf;

class PowerUp
{
public:
    Sprite sprite;
    Texture texture; // Store the texture as a member

    PowerUp(Vector2f startPosition)
    {
        if (!texture.loadFromFile("pacman-art/other/apple.png"))
        {
            std::cerr << "Failed to load apple texture" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(startPosition);
    }
};

class Pickup
{
public:
    CircleShape shape; // Use CircleShape instead of Sprite

    Pickup(Vector2f startPosition)
    {
        shape.setRadius(7.0f);                  // Set the radius of the circle
        shape.setFillColor(Color(255, 165, 0)); // Set the color to orange (RGB: 255, 165, 0)
        shape.setPosition(startPosition);       // Set the position of the circle
    }
};

class Sound_to
{
public:
    // / Global function to play sounds
    void playSound(const std::string &soundFilePath)
    {
        static sf::SoundBuffer buffer;
        static sf::Sound sound;

        if (!buffer.loadFromFile("code/Sounds/eaten.wav"))
        {
            std::cerr << "Failed to load sound: " << soundFilePath << std::endl;
            exit(11);
        }

        sound.setBuffer(buffer);
        sound.play();
    } 
    void background_song(std::string song){
        static sf::Music music;
        // choose an random number between 1 and 5 to choose a random background song
         song = "code/Sounds/background" + song + ".wav";
        if (!music.openFromFile(song))
        {
            std::cerr << "Failed to load music" << std::endl;
            exit(11);
        }
        music.setLoop(true);
        music.play();
    }
    void background_song(){
        static sf::Music music;
        // choose an random number between 1 and 5 to choose a random background song
        int random = rand() % 5 + 1;
        std::string song = "code/Sounds/background" + std::to_string(random) + ".wav";
        if (!music.openFromFile(song))
        {
            std::cerr << "Failed to load music" << std::endl;
            exit(11);
        }
        music.setLoop(true);
        music.play();
    }
    void background_song_nonloop(std:: string song){
        static sf::Music music;
        // choose an random number between 1 and 5 to choose a random background song
        int random = rand() % 5 + 1;
        song = "code/Sounds/background" + song + ".wav";
        if (!music.openFromFile(song))
        {
            std::cerr << "Failed to load music" << std::endl;
            exit(11);
        }
        music.play();
    }

    void background_nosound(){
        static sf::Music music;
        std::string song = "code/Sounds/backgroundstop.wav";
        if (!music.openFromFile(song))
        {
            std::cerr << "Failed to load music" << std::endl;
            exit(11);
        }
        music.stop();
    }
};