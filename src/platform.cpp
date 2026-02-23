#include "../include/platform.h"

Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    SDL_Init(SDL_INIT_VIDEO);
    // SDL_Init(SDL_INIT_AUDIO);

    window = SDL_CreateWindow(title, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
    }

    SDL_SetRenderDrawColor(renderer, 192, 184, 242, 255);

    SDL_SetRenderLogicalPresentation(renderer,
    textureWidth,
    textureHeight,
    SDL_LOGICAL_PRESENTATION_INTEGER_SCALE
    );


    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING,
        textureWidth,
        textureHeight
    );

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_SetTextureColorMod(texture, 15, 3, 92);

   
}

Platform::~Platform() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
};

void Platform::Update(void const* buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
};

bool Platform::ProcessInput(unsigned char* keys)
{
    SDL_Event event;
    bool quit = false;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
                if (event.key.key == SDLK_ESCAPE)
                {
                    quit = true;
                    break;
                }

                int index = MapKeyToIndex(event.key.key);
                if (index != -1)
                {
                    keys[index] = (event.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;
                }
            }
            break;
        }
    }

    return quit;
}


int Platform::MapKeyToIndex(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_X: return 0x0;
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_Q: return 0x4;
        case SDLK_W: return 0x5;
        case SDLK_E: return 0x6;
        case SDLK_A: return 0x7;
        case SDLK_S: return 0x8;
        case SDLK_D: return 0x9;
        case SDLK_Z: return 0xA;
        case SDLK_C: return 0xB;
        case SDLK_4: return 0xC;
        case SDLK_R: return 0xD;
        case SDLK_F: return 0xE;
        case SDLK_V: return 0xF;
        default: return -1;
    }
}