#include "bot.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void report_end_state(Game g, int score, int impossible_moves)
{
	//printf("score: %d, impossible_moves: %d\n", score, impossible_moves);
}

void report_bad_move(Direction d)
{
}

Direction get_input_bot(Game* game)
{
	return rand()%4;
}

int main()
{
	srand(time(0));
	for(int i = 0; i < 100000; i++)
	{
	    play_one_game();
        //usleep(20);
    }
}

