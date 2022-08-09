#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "game.h"
#include "logic.h"
#include "rendering.h"
#include "font.h"

int main(int argc, char *args[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_PNG);
    
    SDL_Window *window;
    SDL_Renderer *renderer;

    window = SDL_CreateWindow(
            "flappy",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
            );
    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
            );

    srand(time(NULL));

    game_t game = {
        .bird = {
            .JUMP_VELOCITY = 11,
            .GRAVITY = 0.65,
            .MAX_VELOCITY = 15,
            .y_position = SCREEN_HEIGHT / 2,
            .y_velocity = 0,
        },
        .state = STARTING_STATE,
        .score = 0,
        .tubes = malloc(sizeof (tube_t) * 2),
        .ground_offset = 0,
        .renderer = renderer,
    };
    generate_tubes(game.tubes);
    init_font(renderer);
    load_textures(renderer);

    Uint64 begin = SDL_GetTicks64();

    while(game.state != QUIT_STATE) {
        Uint64 end = SDL_GetTicks64();
        double time =(double) (end - begin) / 1000; 
        double dt = time * 60;
        begin = end;

        process_input(&game);
        update_game(&game, dt);
        render_game(game.renderer, &game);
    }

    free(game.tubes);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    quit_font();
    free_textures();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

