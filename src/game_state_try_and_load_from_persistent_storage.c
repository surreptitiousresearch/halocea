/* game_state_try_and_load_from_persistent_storage @0x836852A4 — at startup, attempt to restore the
 * saved game state from persistent storage. Skipped during a timedemo. Only proceeds when the stored
 * header reads back, validates, and its difficulty matches the current difficulty; then runs the
 * before-load procs, reads the 0x448000-byte arena into game_state_globals.base_address, reapplies the
 * difficulty, runs the after-load procs, and immediately re-saves.
 * DEVIATION: the disassembly sets only r3 for game_state_header_valid; the 2nd arg is a stale register,
 * not a meaningful flag. */

#include <stdint.h>
#include "headers/game_state_header.h"
#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_state_read_header_from_persistent_storage(void *header, unsigned int *header_checksum, int header_size, int buffer_size, uint8_t *corrupted);
extern uint8_t game_state_header_valid(game_state_header *header, uint8_t fatal);
extern int16_t main_get_difficulty(void);
extern void game_state_call_before_load_procs(void);
extern void game_state_read_from_persistent_storage(void *buffer, unsigned int buffer_size);
extern void game_difficulty_level_set(int16_t difficulty);
extern void game_state_call_after_load_procs(void);
extern void game_state_save(uint8_t do_autosave);

void game_state_try_and_load_from_persistent_storage(void)
{
    game_state_header header;
    if ( !timeDemo
      && game_state_read_header_from_persistent_storage(&header, &header.checksum, 332, 4489216, 0)
      && game_state_header_valid(&header, 0)   /* 2nd arg stale in caller */
      && main_get_difficulty() == header.difficulty )
    {
        game_state_call_before_load_procs();
        game_state_read_from_persistent_storage(game_state_globals.base_address, 4489216);
        game_difficulty_level_set(main_get_difficulty());
        game_state_call_after_load_procs();
        game_state_save(0);
    }
}
