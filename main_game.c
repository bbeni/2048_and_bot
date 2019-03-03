#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 4

#ifdef BOT_PLAYING
void end_of_game(score, impossible_moves);
void report_bad_move();
Direction get_input_bot(Game* game);
#endif

typedef struct Game {
    unsigned int board [SIZE][SIZE];
} Game;

typedef enum {
    UP, RIGHT, DOWN, LEFT
} Direction;

unsigned int random_value()
{
    if (rand()%10 == 0)
        return 4;
    return 2;
}

void combine_adjacents(Direction d, Game* game);
void collapse_zeros(Direction d, Game* game);
void transpose_board(Direction d, Game* g);
void move(Direction d, Game* g);

void place_at_random_free_place(unsigned int value, Game* g)
{
    while(1)
    {
        int i = rand()%SIZE;
        int j = rand()%SIZE;
        if( g->board[i][j] == 0)
        {
            g->board[i][j] = value;
            return;
        }
    }
}

bool is_full(Game* g)
{
    // fast check if a 0 is in there
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<SIZE; j++)
            if (g->board[i][j] == 0) return false;

    // just try every move and see if it does something ;)
    Game g2 = *g;
    move(UP, &g2);
    move(RIGHT, &g2);
    move(DOWN, &g2);
    move(LEFT, &g2);
    if (memcmp(g2.board, g->board, sizeof(g)) == 0)
        return true;
    return false;
}

void move(Direction d, Game* game)
{
    transpose_board(d, game);
    collapse_zeros(d, game);
    combine_adjacents(d, game);
    collapse_zeros(d, game);
    transpose_board(d, game);
}

void init_vars(int* end, int* j, int* inc, Direction d)
{
    switch(d) {
        case RIGHT:
        case DOWN: *end = 0; *j = SIZE-1; *inc = -1; break;
        case LEFT:
        case UP: *end = SIZE-1; *j = 0; *inc = +1; break;
    }
}

void transpose_board(Direction d, Game* g)
{
    if (d==DOWN || d==UP)
    {
        Game g2 = {};
        for(int i=0; i<SIZE; i++)
            for(int j=0; j<SIZE; j++)
                g2.board[i][j] = g->board[j][i];
        *g = g2;
    }
}


void collapse_zeros(Direction d, Game *game)
{
    for (int row=0; row<SIZE; row++)
    {
        // decide the variables depending on the direction
        int end, j, inc;
        init_vars(&end, &j, &inc, d);

        for ( ;j!=end;j+=inc)
        {
            if (game->board[row][j]==0)
            {
                for(int a=j; a!=end; a+=inc)
                    game->board[row][a] = game->board[row][a+inc];
                game->board[row][end] = 0;
                end -= inc;
                j -= inc;
            }
        }
    }
}

void combine_adjacents(Direction d, Game* game)
{
    for(int row=0;row<SIZE;row++)
    {
        // decide the variables depending on the direction
        int end, j, inc;
        init_vars(&end, &j, &inc, d);

        for (; j!=end; j+=inc)
        {
            unsigned int x = game->board[row][j];
            unsigned int y = game->board[row][j+inc];
            if (x==y)
            {
                game->board[row][j] *= 2;
                game->board[row][j+inc] = 0;
            }
        }
    }
}

// user stuff... 

void print(Game* g)
{
    for (int i=0; i<SIZE; i++)
    {
        for (int j=0; j<SIZE; j++)
        {
            printf("%d\t", g->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

Direction get_input_user()
{
    // asdw input

    printf("a, s, d, w to move >>  \n");
    int c = 0;

    system ("/bin/stty raw");
    while(true) 
    {
        c = getchar();
        if (c==119 || c==100 || c==115 || c==97 )
            break;
    }
    system ("/bin/stty cooked");

    switch(c)
    {
        case 119: return UP;
        case 100: return RIGHT;
        case 115: return DOWN;
        case 97: return LEFT;
    }
}


int main()
{
    srand(time(0));

    Game game = {};
    place_at_random_free_place(random_value(), &game);
    place_at_random_free_place(random_value(), &game);

    int score = 0;
    int impossible_moves = 0;

    do {
        print(&game);

#ifdef BOT_PLAYING
        Direction d = get_input_bot(Game& game);
#else
        Direction d = get_input_user();
        printf("direction: %d\n", d);
#endif

        // save a copy before movement
        Game g2 = game;
        move(d, &game);

        if (memcmp(game.board, g2.board, sizeof(game)) == 0)
        {
            printf("Bad move!!\n");
#ifdef BOT_PLAYING
            report_bad_move();
#endif
            impossible_moves+=1;
            continue;
        }

        score += 1;
        place_at_random_free_place(random_value(), &game);

    } while(!is_full(&game));

    print(&game);
    printf("score: %d, bad moves: %d\n", score, impossible_moves);

#ifdef BOT_PLAYING
    end_of_game(score, impossible_moves);
#endif 


    return 0;
}
