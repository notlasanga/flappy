#ifndef LOGIC_H
#define LOGIC_H

void generate_tubes(tube_t *tubes);

void reset_game(game_t *game);

void process_input(game_t *game);

void update_game(game_t *game, double dt);

#endif // LOGIC_H
