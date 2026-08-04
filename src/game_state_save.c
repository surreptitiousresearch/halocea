/* game_state_save @0x83684908 — checkpoint the game: run the before-save procs, stop the clock, write the
 * state to file (recording whether the resulting saved game is valid), then restart the clock.
 *
 * Deviation: before_save_procs[0] is an array of void(*)(void) callbacks; the decompiler's leftover return
 * value feeding main_stop_time is ABI noise (main_stop_time takes no argument) — reproduced as independent
 * calls. */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern void (*before_save_procs[])(void);
extern void main_stop_time(void);
extern void main_start_time(void);
extern uint8_t game_state_write_to_file(uint8_t do_autosave);

void game_state_save(uint8_t do_autosave)
{
    before_save_procs[0]();
    main_stop_time();
    game_state_globals.saved_game_valid = game_state_write_to_file(do_autosave) != 0;
    main_start_time();
}
