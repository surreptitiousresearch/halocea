/* game_initial_pulse @0x83688230 — fire the initial game-engine events at game start: announce each
 * existing player, then signal that the game is starting. */

#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void game_engine_player_added(int player_index);
extern void game_engine_game_starting(void);

/* attested void: 0/4 callers consume r3 — the trailing game_engine_game_starting r3 is callee residue */
void game_initial_pulse(void)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    while ( data_iterator_next(&iterator) )
        game_engine_player_added(iterator.index);
    game_engine_game_starting();
}
