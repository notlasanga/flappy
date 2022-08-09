#include <SDL2/SDL.h>
#include <stdbool.h>

#include "game.h"
#include "logic.h"

void update_bird(game_t *game, bird_t *bird, double dt) {
    if (bird->isJumping) {
        bird->y_velocity = -1 * bird->JUMP_VELOCITY; 
        bird->isJumping = false;
    } else {
        bird->y_velocity += bird->GRAVITY * dt;
        if (bird->y_velocity > bird->MAX_VELOCITY) {
            bird->y_velocity = bird->MAX_VELOCITY;
        }
    }
    bird->y_position += bird->y_velocity * dt;
}

void update_tubes(game_t *game, double dt) {
    for (int i = 0; i < TUBE_COUNT; i++) {
        game->tubes[i].position -= SPEED * dt;
        if (game->tubes[i].position + TUBE_WIDTH / 2 + SPEED * dt >= SCREEN_WIDTH / 2 && game->tubes[i].position + TUBE_WIDTH / 2 < SCREEN_WIDTH / 2) {
            game->score++;
        } else if (game->tubes[i].position < -(TUBE_WIDTH + 8)) {
            game->tubes[i].position = SCREEN_WIDTH + 8 - fmod(-game->tubes[i].position - TUBE_WIDTH + 8, SCREEN_WIDTH + TUBE_WIDTH);
            game->tubes[i].height = TUBE_MIN_HEIGHT + (rand() % TUBE_MAX_HEIGHT);
        }
    }
    printf("");
}

void generate_tubes(tube_t *tubes) {
    for (int i = 0; i < TUBE_COUNT; i++) {
        tubes[i].position = SCREEN_WIDTH * 2 + (SCREEN_WIDTH + TUBE_WIDTH + 8) / 2 * i;
        tubes[i].height = TUBE_MIN_HEIGHT + (rand() % TUBE_MAX_HEIGHT);
    }
}

void check_collisions(game_t *game) {
    game->bird.isColliding = false;
    for (int i = 0; i < TUBE_COUNT; i++) {
        if (game->tubes[i].position < SCREEN_WIDTH / 2 + BIRD_WIDTH / 2 &&
            game->tubes[i].position > SCREEN_WIDTH / 2 - TUBE_WIDTH - BIRD_WIDTH / 2) {
            if (game->bird.y_position > SCREEN_HEIGHT - game->tubes[i].height - BIRD_HEIGHT / 2 || 
                game->bird.y_position < SCREEN_HEIGHT - (game->tubes[i].height + TUBE_GAP) + BIRD_HEIGHT / 2) {
                game->state = GAME_OVER_STATE;
                return;
            }
        }
    }
    if (game->bird.y_position > SCREEN_HEIGHT - BIRD_HEIGHT / 2 - GROUND_HEIGHT) {
        game->state = GAME_OVER_STATE;
    }
}

void update_ground(game_t *game, double dt) {
    game->ground_offset += SPEED * dt;
    game->ground_offset = fmod(game->ground_offset, 24);
}

void reset_game(game_t *game) {
    game->state = STARTING_STATE;
    game->bird.y_position = SCREEN_HEIGHT / 2;
    game->bird.y_velocity = 0;
    game->bird.isColliding = false;
    game->score = 0;
    game->bird.rotation = 0;
    generate_tubes(game->tubes);
}

void process_input(game_t *game) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            game->state = QUIT_STATE;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    game->bird.isJumping = true;
                    break;
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            game->bird.isJumping = true;
        }
    }
}

void update_game(game_t *game, double dt) {
    switch (game->state) {
        case (RUNNING_STATE):
            update_tubes(game, dt);
            update_bird(game, &game->bird, dt);
            check_collisions(game);
            update_ground(game, dt);
            break;
        case (STARTING_STATE):
            if (game->bird.isJumping) {
                game->state = RUNNING_STATE;
            }
            update_ground(game, dt);
            break;
        case (GAME_OVER_STATE):
            if (game->bird.y_position < SCREEN_HEIGHT - GROUND_HEIGHT - BIRD_HEIGHT / 2) {
                game->bird.y_velocity = game->bird.MAX_VELOCITY;
                game->bird.y_position += game->bird.y_velocity * dt;
            } else if (game->bird.isJumping) {
                reset_game(game);
                game->bird.isJumping = false;
            }
            break;
    }
}
