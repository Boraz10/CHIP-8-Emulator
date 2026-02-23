#pragma once
#include <cstdio>
#include <fstream>
#include <chrono>
#include <random>
#include <cstring>
#include <iostream>
#include <vector>

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

#define START_ADDRESS 0x200
#define FONT_START_ADDRESS 0x050

//TODO: Put some other definitions here for better readability

// Typedefs
typedef unsigned char BYTE;
typedef unsigned short WORD;




class chip8 {
    public:
        chip8();                            // Constructor. Initialises memory and all other aspects

        void Cycle();                       // Once cycle of code.

        void loadGame(const char* game);    // Loads game from file input

        uint32_t gfx[DISPLAY_WIDTH * DISPLAY_HEIGHT];   // display

        BYTE keypad[16];                            // keyboard input


    
     
    private:
        WORD opcode;                                // Code of current operation to perform. Nescesary data included in code.

        BYTE memory[4096];                          // chip8 memory (4K)
        BYTE V[16];                                 // registers. V[F] is reserved for flags

        WORD I;                                     // index
        WORD pc;                                    // program counter


        BYTE delay_timer;                           // Delay timer. When not 0, counts down at 60hz
        BYTE sound_timer;                           // Timer for sound. When not 0, plays a tone. Counts down at 60hz

        WORD stack[16];                             // stack
        WORD sp;                                    // stack pointer


        // Random number generation
        std::default_random_engine randGen;
        std::uniform_int_distribution<BYTE> randByte;

        // opcode functions
        void OP_00E0(); void OP_00EE(); void OP_0nnn();
        void OP_1nnn(); void OP_2nnn(); void OP_3xkk(); void OP_4xkk(); 
        void OP_5xy0(); void OP_6xkk(); void OP_7xkk(); void OP_8xy0();
        void OP_8xy1(); void OP_8xy2(); void OP_8xy3(); void OP_8xy4();
        void OP_8xy5(); void OP_8xy6(); void OP_8xy7(); void OP_8xyE();
        void OP_9xy0(); void OP_Annn(); void OP_Bnnn(); void OP_Cxkk();
        void OP_Dxyn(); void OP_Ex9E(); void OP_ExA1(); void OP_Fx07();
        void OP_Fx0A(); void OP_Fx15(); void OP_Fx18(); void OP_Fx1E();
        void OP_Fx29(); void OP_Fx33(); void OP_Fx55(); void OP_Fx65();

        // null opcode (default)
        void OP_NULL() {};

        // table functions for opcodes with same first digit
        void Table0(); void Table8(); void TableE(); void TableF(); 

        // function tables
        typedef void (chip8::*Chip8Func)();         // Chip8func is a pointer to a function. We have arrays of these that point to each opcode
        Chip8Func table[0xF + 1];                   // Unique opcodes. Goes from 0x0 to 0xF
        Chip8Func table0[0xE + 1];                  // Opcodes where first digit is 0. Goes up to 0xE
        Chip8Func table8[0xE + 1];                  // Opcodes where first digit is 8. Goes up to 0xE
        Chip8Func tableE[0xE + 1];                  // Opcodes where the first digit is E. Goes uo to 0xE
        Chip8Func tableF[0x65 + 1];                 // Opcodes where the first digit is F. Goes up to 0x65

        BYTE chip8_fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };  

};