#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#define WIDTH 1280
#define HEIGHT 720
#define FRAMERATE 60
static const int YUV_FRAME_SIZE = (int)(WIDTH * HEIGHT * 1.5);

int main(){

    // initialize sdl
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL INITIALIZATION ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    // create sdl window centered on screen
    SDL_Window* window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    // enables hardware acceleration
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, render_flags);

    // creates texture to hold YUV video
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);

    // command to output raspiberry pi camera in yuv420 format
    char* command;
    if(asprintf(&command, "rpicam-vid --codec yuv420 -t 0 --width %d --height %d -o - -n --framerate %d", WIDTH, HEIGHT, FRAMERATE) == -1){
        printf("rpicam-vid command ERROR\n");
        free(command);
        exit(1);
    }

    // pipe for video data
    FILE *pipe = popen(command, "r");
    if(!pipe){
        printf("Pipe creation error\n");
        pclose(pipe);
        exit(1);
    }

    // creates event to handle inputs
    SDL_Event event;
    // input loop
    while(1){
        // check for exit
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                // clean up
                free(command);
                pclose(pipe);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                return 0;
            }
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if(keystates[SDL_SCANCODE_W]){
            printf("Forward\n");
        }
        if(keystates[SDL_SCANCODE_A]){
            printf("Left\n");
        }
        if(keystates[SDL_SCANCODE_S]){
            printf("Back\n");
        }
        if(keystates[SDL_SCANCODE_D]){
            printf("Right\n");
        }

        // read framebuffer from pipe
        char frameBuffer[YUV_FRAME_SIZE];
        fread(frameBuffer, 1, YUV_FRAME_SIZE, pipe);

        // update texture with frame data
        SDL_UpdateTexture(texture, NULL, frameBuffer, WIDTH);

        // clear rendere
        SDL_RenderClear(renderer);

        // copy texture to renderer
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // display renderer to window
        SDL_RenderPresent(renderer);
    }


    // clean up
    free(command);
    pclose(pipe);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}