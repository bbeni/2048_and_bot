#ifndef game_h
#define game_h
#define SIZE 4

typedef struct Game {
    unsigned int board [SIZE][SIZE];
} Game;

typedef enum {
    UP, RIGHT, DOWN, LEFT
} Direction;

int play_one_game();

void combine_adjacents(Direction d, Game* game);
void collapse_zeros(Direction d, Game* game);
void transpose_board(Direction d, Game* g);
void move(Direction d, Game* g);

#endif