#include "code/pacman.cpp" // Include the Pacman game logic

int main() {
    // Create a Pacman game object and run the game
    srand(static_cast<unsigned>(time(0)));

    PacmanGame game;
    game.run();
    return 0;
}
