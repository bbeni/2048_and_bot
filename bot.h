#ifndef bot_h
#define bot_h
#include "game.h"

void report_end_state(Game game, int score, int impossible_moves);
void report_bad_move(Direction d);
Direction get_input_bot(Game* game);

#endif