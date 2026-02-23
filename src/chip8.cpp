#include "../include/chip8.h"

chip8::chip8()
: randGen(std::chrono::system_clock::now().time_since_epoch().count())
 { // constructor

    // initialise registers and memory once

    pc     = (WORD)START_ADDRESS;    // where the program starts
    opcode = 0;                      // Reset opcode
    I      = 0;                      // Reset index register
    sp     = 0;                      // reset stack pointer 

    // Clear display
    memset(gfx, 0, sizeof(gfx));

    for (size_t i = 0; i < 16; i++) {
        stack[i] = 0;     // Clear stack
        V[i]     = 0;     // Clear Registers V0 - VF
    }


    // Clear memory
    for (size_t i = 0; i < 4096; i++) {
        memory[i] = 0;
    }

    // Load fontset into correct registers
    for (int i = 0; i < 80; ++i) {
        memory[FONT_START_ADDRESS + i] = chip8_fontset[i];
    }

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Initialise RNG
    randByte = std::uniform_int_distribution<BYTE>(0, 255U); // Generates random number between 0 and 255

    // Populate function pointer tables

    // Function pointer table
    table[0x0] = &chip8::Table0;            // Opcodes that start with 0
    table[0x1] = &chip8::OP_1nnn;
    table[0x2] = &chip8::OP_2nnn;
    table[0x3] = &chip8::OP_3xkk;
    table[0x4] = &chip8::OP_4xkk;
    table[0x5] = &chip8::OP_5xy0;
    table[0x6] = &chip8::OP_6xkk;
    table[0x7] = &chip8::OP_7xkk;
    table[0x8] = &chip8::Table8;           // Opcodes that start with 8
    table[0x9] = &chip8::OP_9xy0;
    table[0xA] = &chip8::OP_Annn;
    table[0xB] = &chip8::OP_Bnnn;
    table[0xC] = &chip8::OP_Cxkk;
    table[0xD] = &chip8::OP_Dxyn;
    table[0xE] = &chip8::TableE;            // Opcodes that start with E
    table[0xF] = &chip8::TableF;            // Opcodes that start with F

    // poulate tables 0, 8 and E with OP_NULL by default (since not all opcodes in the table exist)
    for (size_t i = 0; i <= 0xE; ++i) {
        table0[i] = &chip8::OP_NULL;
        table8[i] = &chip8::OP_NULL;
        tableE[i] = &chip8::OP_NULL;
    }

    // Put 0 opcodes in table0 at correct place
    table0[0x0] = &chip8::OP_00E0;
    table0[0xE] = &chip8::OP_00EE;

    // Put 8 opcodes in table8 at correct place
    table8[0x0] = &chip8::OP_8xy0;
    table8[0x1] = &chip8::OP_8xy1;
    table8[0x2] = &chip8::OP_8xy2;
    table8[0x3] = &chip8::OP_8xy3;
    table8[0x4] = &chip8::OP_8xy4;
    table8[0x5] = &chip8::OP_8xy5;
    table8[0x6] = &chip8::OP_8xy6;
    table8[0x7] = &chip8::OP_8xy7;
    table8[0xE] = &chip8::OP_8xyE;

    // Put E opcodes in tableE at correct place
    tableE[0x1] = &chip8::OP_ExA1;
    tableE[0xE] = &chip8::OP_Ex9E;


    // Populate table F with OP_NULL by default
    for (size_t i = 0; i < 0x65; ++i) {
        tableF[i] = &chip8::OP_NULL;
    }

    // Put F opcodes in tableF at correct place
    tableF[0x07] = &chip8::OP_Fx07;
    tableF[0x0A] = &chip8::OP_Fx0A;
    tableF[0x15] = &chip8::OP_Fx15;
    tableF[0x18] = &chip8::OP_Fx18;
    tableF[0x1E] = &chip8::OP_Fx1E;
    tableF[0x29] = &chip8::OP_Fx29;
    tableF[0x33] = &chip8::OP_Fx33;
    tableF[0x55] = &chip8::OP_Fx55;
    tableF[0x65] = &chip8::OP_Fx65;


};

void chip8::Cycle() {
    // Fetch opcode
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    // Increment PC before executing
    pc += 2;

    // Decode and execute opcode
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();
    
    // Update timers
    if (delay_timer > 0) --delay_timer;
    if (sound_timer > 0) --sound_timer;

};


void chip8::loadGame(const char* game) {
    // open game as ifstream in binary mode
    std::ifstream file(game, std::ios::binary | std::ios::ate);

    // Throw error if file does not open
    if (!file.is_open()) {
        perror("Could not open file");
        exit(1);
    }

    // get file size
    std::streampos size = file.tellg();

    // create buffer to hold contents in the program on heap
    char* buffer = new char[size];

    // load file contents into buffer
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    // load ROM contents into memory
    for (long i = 0; i < size; ++i) {
        memory[START_ADDRESS + i] = buffer[i];
    }

    // free buffer
    delete[] buffer;
};


// Opcodes

void chip8::OP_00E0() {
    // Clear Display
    memset(gfx, 0, sizeof(gfx));
};

void chip8::OP_00EE() {
    // Return from subroutine
    --sp;
    pc = stack[sp];

};

void chip8::OP_0nnn() {};

void chip8::OP_1nnn() {
    // Jump to location nnn

    WORD address = opcode & 0x0FFFu;

    pc = address;
};
void chip8::OP_2nnn() {
    // Call subroutine at nnn
    WORD address = opcode & 0x0FFFu;

    stack[sp] = pc;
    ++sp;
    pc = address;

};

void chip8::OP_3xkk() {
    // Skip next instruction if Vx == kk
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE byte = opcode & 0x00FFu;
    if(V[x] == byte) pc+=2;
};

void chip8::OP_4xkk() {
    // Skip next instruction if Vx != kk
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE byte = opcode & 0x00FFu;
    if(V[x] != byte) pc+=2;
}; 

void chip8::OP_5xy0() {
    // Skip next instruction if Vx == Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    if (V[x] == V[y]) pc+=2;
};

void chip8::OP_6xkk() {
    // Set Vx = kk
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE byte = opcode & 0x00FFu;
    V[x] = byte;
};

void chip8::OP_7xkk() {
    // Set Vx = Vx + kk
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE byte = opcode & 0x00FFu;
    V[x] += byte;
};

void chip8::OP_8xy0() {
    // Set Vx = Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    V[x] = V[y];
};

void chip8::OP_8xy1() {
    // Set Vx = Vx OR Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    V[x] |= V[y];
};

void chip8::OP_8xy2() {
    // Set Vx = Vx AND Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    V[x] &= V[y];
}; 

void chip8::OP_8xy3() {
    // Set Vx = Vx XOR Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    V[x] ^= V[y];
};

void chip8::OP_8xy4() {
    // Set Vx = Vx + Vy, set VF = carry (1 if Vx + Vy is greater than 255)
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    WORD sum = V[x] + V[y];

    if (sum > 255u) V[0xF] = 1;
    else V[0xF] = 0;

    V[x] = sum & 0xFFu; // only lowest 8 bits. I think, maybe, i can just add them but idk
    
};

void chip8::OP_8xy5() {
    // Set Vx = Vx - Vy, set VF = NOT borrow.
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    if (V[x] > V[y]) V[0xF] = 1;
    else V[0xF] = 0;
    V[x] -= V[y];
};

void chip8::OP_8xy6() {
    // Set Vx = Vx SHR 1
    BYTE x = (opcode & 0x0F00u) >> 8u;

    // Basically sotre lsb in V[f]
    V[0xF] = (V[x] & 0x1u);

    // perform a right shift to divide by 2.
    V[x] >>= 1;

};

void chip8::OP_8xy7() {
    // Set Vx = Vy - Vx, set VF = NOT borrow
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    if (V[y] > V[x]) V[0xF] = 1;
    else V[0xF] = 0;

    V[x] = V[y] = V[x];
};

void chip8::OP_8xyE() {
    // Set Vx = Vy SHL 1
    BYTE x = (opcode & 0x0F00u) >> 8u;

    V[0xF] = (V[x] & 0x80u) >> 7u;

    V[x] <<= 1;
};

void chip8::OP_9xy0() {
    // Skip next instruction if Vx != Vy
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    if (V[x] != V[y]) pc +=2;
}; 

void chip8::OP_Annn() {
    // Set I = nnn
    BYTE address = opcode & 0x0FFFu;
    I = address;
};
void chip8::OP_Bnnn() {
    // Jump to location nnn + V0
    BYTE address = (opcode & 0x0FFFu);
    pc = address = V[0];
};
void chip8::OP_Cxkk() {
    // Set Vx = random byte AND kk;
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE byte = (opcode & 0x00FFu);
    V[x] = randByte(randGen) & byte;
};
void chip8::OP_Dxyn() {
    // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE y = (opcode & 0x00F0u) >> 4u;
    BYTE n = (opcode & 0x000Fu);

    // Wrap if we go beyond boundries
    BYTE xPos = V[x] % DISPLAY_WIDTH;
    BYTE yPos = V[y] % DISPLAY_HEIGHT;

    V[0xF] = 0;

    for (size_t row = 0; row < n; ++row) {
        BYTE spriteByte = memory[I + row];

        for (size_t col = 0; col < 8; ++col) {
            BYTE spritePixel = spriteByte & (0x80u >> col);
            BYTE* screenPixel = &gfx[(yPos + row) * DISPLAY_WIDTH + (xPos + col)]; // TODO: potential typing issue

            // sprite pixel is on
            if (spritePixel) {

                // screen pixel also on
                if (*screenPixel == (BYTE)0xFF) {
                    V[0xf] = 1;
                }

                // XOR withsprite pixel
                *screenPixel ^= 0xFF;
            }
        }
    }
};

void chip8::OP_Ex9E() {
    // Skip next instruction if key with value of Vx is pressed
    BYTE x = (opcode & 0x0F00u) >> 8u;

    BYTE key = V[x];

    if (keypad[key]) {
        pc += 2;
    }
};

void chip8::OP_ExA1() {
    // Skip next instruction if key with value of Vx i nots pressed
    BYTE x = (opcode & 0x0F00u) >> 8u;

    BYTE key = V[x];

    if (!keypad[key]) {
        pc += 2;
    }
};

void chip8::OP_Fx07() {
    // Set Vx = Dispay Timer value
    BYTE x = (opcode & 0x0F00u) >> 8u;
    V[x] = delay_timer;
};

void chip8::OP_Fx0A() {
    // Wait for a key press, store the value of the key in Vx
    BYTE x = (opcode & 0x0F00u) >> 8u;

    for (size_t i = 0; i < 16; ++i) {
        if (keypad[i]) {
            V[x] = i;
            return;
        }
    }
    pc -= 2;

};
void chip8::OP_Fx15() {
    // Set delay timer = Vx
    BYTE x = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[x];
};

void chip8::OP_Fx18() {
    // Set sound timer = Vx;
    BYTE x = (opcode & 0x0F00u) >> 8u;
    sound_timer = V[x];
};
void chip8::OP_Fx1E() {
    // Set I = I + Vx
    BYTE x = (opcode & 0x0F00u) >> 8u;
    I += V[x];
};
void chip8::OP_Fx29() {
    // Set I = location of sprite for digit Vx
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE digit = V[x];

    I = FONT_START_ADDRESS + (5 * digit);


};
void chip8::OP_Fx33() {
    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
    BYTE x = (opcode & 0x0F00u) >> 8u;
    BYTE value = V[x];

     // Ones place
     memory[I + 2] = value % 10;
     value /= 10;

     // Tens place
     memory[I + 1] = value % 10;
     value /= 10;

     // Hundreds place
     memory[I] = value % 10;
};
void chip8::OP_Fx55() {
    // Store registers V0 through Vx in memory starting at location I.
    BYTE x = (opcode & 0x0F00u) >> 8u;

    for (BYTE i = 0; i < x; ++i) {
        memory[I + i] = V[i];
    }
};
void chip8::OP_Fx65() {
    // Read registers V0 through Vx from memory starting at location I.
    BYTE x = (opcode & 0x0F00u) >> 8u;
    for (BYTE i = 0; i < x; ++i) {
        V[i] = memory[I + i];
    }
};


// Table functions for opcoeds with common first digit

void chip8::Table0() {
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void chip8::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}