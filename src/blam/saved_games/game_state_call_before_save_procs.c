/* game_state_call_before_save_procs @0x83684480 — invoke the first registered before-save proc.
 * before_save_procs holds void(void) callbacks (see blam_data_globals.h / game_state_save.c), so
 * this proc is void — the decompiler's threaded "return" was uninitialized r3 residue. */

#include "headers/blam_data_globals.h"

void game_state_call_before_save_procs(void)
{
    before_save_procs[0]();
}
