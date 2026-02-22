#pragma once
# include <cstdio>
# include <fstream>
#include <chrono>
#include <random>

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 32

#define START_ADDRESS 0x200
#define FONT_START_ADDRESS 0x200

//TODO: Put some other definitions here for better readability

// Typedefs
typedef unsigned char BYTE;
typedef unsigned short WORD;



class chip8 {
    public:
        chip8();

        void emulateCycle();

        void loadGame(const char* game);

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

    private:
        WORD opcode;

        BYTE memory[4096];  // memory
        BYTE V[16];         // registers

        BYTE I;             // index
        WORD pc;            // program counter

        BYTE gfx[DISPLAY_WIDTH * DISPLAY_HEIGHT];   // display

        BYTE delay_timer;
        BYTE sound_timer;

        WORD stack[16];     // stacl
        WORD sp;            // stack pointer

        BYTE keypad[16];   // keys

        BYTE key[16];

        const BYTE chip8_fontset[80] = {
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

        // Random number generation
        std::default_random_engine randGen;
        std::uniform_int_distribution<BYTE> randByte;

};