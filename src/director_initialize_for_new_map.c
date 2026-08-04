/* director_initialize_for_new_map @0x836E5858 — reset the camera director for a new map: set the game mode
 * (gameplay vs editor), then for each local player clear the camera-change/boredom state, choose an initial
 * camera, and seed the debug camera variables from their definitions' initial values.
 *
 * Deviation: the decompiler addresses debug_variables via raw offsets from camera_mode_index (stride 12);
 * reproduced with debug_variables[] indexing. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_game_mode.h"
#include "headers/director_variable_definition.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_in_editor(void);
extern void director_choose_camera(int16_t local_player_index, uint8_t initialize, uint8_t key);

extern void director_initialize_variables(int16_t local_player_index);
void director_initialize_for_new_map(void)
{
    director_globals.initialize_camera = 0;
    director_globals.game_mode = game_in_editor() == 0 ? _director_mode_game : _director_mode_editor;

    for ( __int16 player = 0; player < 2; player = (__int16)(player + 1) )
    {
        director *d = &director_globals.local_players[player];
        d->camera_change_pause = 0.0f;
        d->bored = 0;
        d->bored_time = 0;
        director_choose_camera(player, 1u, 0);
        /* DEVIATION: director_initialize_variables@0x836E4930 (zero-xref, fully inlined) reconstructed here — local_player_index binds to the loop var `player` (not constant-folded, since the call is per-player); collapsed the verbatim 4-field reset loop back to a call. */
        director_initialize_variables(player);
    }
}
