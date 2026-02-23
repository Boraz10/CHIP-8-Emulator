#include <iostream>
#include <cstring>
#include <chrono>
#include <string>
// Need to include something for graphics and input

#include "../include/chip8.h"
#include "../include/platform.h"

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

int main(int argc, char *argv[]) {
     if (argc != 2 && argc != 4) {
        std::cout << "Usage:"                                                               << std::endl;
        std::cout << "      chip8 <game>\n"                                                 << std::endl;
        std::cout << "Games are in the /games folder, adjacent to the executable"           << std::endl;
        std::cout << "If you want to add games, put them in this folder"                    << std::endl;
        std::cout << "You don't need to specify the folder, only the name of the game."     << std::endl;
        std::cout << "\nExample:"                                                           << std::endl;
        std::cout << "      chip8 pong\n"                                                   << std::endl;
        std::cout << "Optionally, you may alse set the scale and delay. Both must be set:"  << std::endl;
        std::cout << "      chip8 <game> <scale> <delay>\n"                                 << std::endl;
        
        exit(1);
    }

    int scale = 10;
    int delay = 1;

    if (argc == 4) {
        scale = std::stoi(argv[2]);
        delay = std::stoi(argv[3]);
    }

    // Initialise platform
    Platform platform("Chip-8 Emulator", DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    std::cout << "platform inititialised" << std::endl;

    // Initialise Chip8 
    chip8 Chip8;
    
    std::cout << "chip8 inititialised" << std::endl;

    // Load rom
    const char* games_directory = "games/";

    size_t rom_name_len = std::strlen(games_directory) + std::strlen(argv[1]);

    char* rom = new char[rom_name_len];
    
    std::strcpy(rom, games_directory);
    std::strcat(rom, argv[1]);
    std::strcat(rom, ".ch8");

     Chip8.loadGame(rom);

     delete[] rom;

    std::cout << "rom loaded" << std::endl;

     int videoPitch = sizeof(Chip8.gfx[0]) * DISPLAY_WIDTH;

     auto lastCycleTime = std::chrono::high_resolution_clock::now();
     bool quit = false;
   
    

    // Emulation loop
    while (!quit) {
        quit = platform.ProcessInput(Chip8.keypad);

        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
        
        if (dt > delay) {
            lastCycleTime = currentTime;

            Chip8.Cycle();

            platform.Update(Chip8.gfx, videoPitch);
        }

    }

    return 0;
}