#pragma once
#include <SDL3/SDL.h>

class Platform {
    public:
        Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
        ~Platform();

        void Update(void const* buffer, int pitch);

        bool ProcessInput(unsigned char* keys);

        int MapKeyToIndex(SDL_Keycode key);

    private:
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};
    
};