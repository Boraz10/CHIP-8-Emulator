#pragma once
// Typedefs
typedef unsigned char BYTE;
typedef unsigned short WORD;



class chip8 {
    public:

    private:
        WORD opcode;

        BYTE memory[4096];
        BYTE V[16];

        BYTE I;
        BYTE pc;

        BYTE gfx[64][32];

        BYTE delay_timer;
        BYTE sound_timer;

        WORD stack[16];
        WORD sp;

        BYTE key[16];

};