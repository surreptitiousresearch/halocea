/* game_state_save_to_persistent_storage @0x83684AE0 — on the very first player spawn, revert to the
 * last saved state then write the current game state header region out to persistent storage. */

#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"

extern void game_state_revert(void); /* attested void */
extern void game_state_write_to_persistent_storage(void *buffer, unsigned int *header_checksum, int header_size, int buffer_size);

void game_state_save_to_persistent_storage(void)
{
    if ( player_spawn_count == 1 )
    {
        game_state_revert();
        game_state_write_to_persistent_storage(
            game_state_globals.base_address,
            &game_state_globals.header->checksum,
            332,
            4489216);
    }
}
