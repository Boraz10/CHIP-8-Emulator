#include <iostream>
#include <vector>
// Need to include something for graphics and input

#include "../include/chip8.h"

int main(int argc, char *argv[]) {
     if (argc != 2) {
        std::cout << "Usage:"                                                           << std::endl;
        std::cout << "      chip8 <game>\n"                                             << std::endl;
        std::cout << "Games are in the /games folder, adjacent to the executable"       << std::endl;
        std::cout << "If you want to add games, put them in this folder"                << std::endl;
        std::cout << "You don't need to specify the folder, only the name of the game." << std::endl;
        std::cout << "\nExample:"                                                       << std::endl;
        std::cout << "      chip8 pong\n"                                               << std::endl;
        
        exit(1);
    }

    // Initialise Chip8 
    chip8 Chip8;

    // Set up graphics
    // Set up input


    // Load game into memory
    Chip8.loadGame(argv[1]);
    

    // Emulation loop
    for (;;) {
        // Emulate once cycle

        // If draw flag is set, update the screen

        // Store key press state, press and release.

    }

    return 0;
}