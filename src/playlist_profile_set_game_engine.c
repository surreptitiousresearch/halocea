/* playlist_profile_set_game_engine @0x83692E88 — playlist-editor UI handler: map the selected game-type
 * list row of the parent widget onto the edit playlist profile's game engine (1 = slayer default; row 1 →
 * king (4), row 2 → ctf (2), row 3 → oddball (3), row 4 → race (5); rows past 4 keep the current engine;
 * row 0 keeps slayer and turns teams on). When the engine actually changes, the 24-byte engine-specific
 * variant union (the 6 ints following time_limit — the compiled pointer loop) is cleared. Returns 0 only
 * when no profile is being edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);

uint8_t playlist_profile_set_game_engine(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *profile = player_ui_get_edit_playlist_profile();
    if ( !profile )
        return 0;

    int game_engine_index = game_engine_ctf;
    unsigned int row = (unsigned __int16)widget->parent->parameters.text_box_parameters.string_list_index;
    if ( row > 4 )
        game_engine_index = profile->game_engine_index;
    else if ( row == 1 )
        game_engine_index = game_engine_king;
    else if ( row == 2 )
        game_engine_index = game_engine_slayer;
    else if ( row == 3 && widget->parent->parameters.text_box_parameters.string_list_index )
        game_engine_index = game_engine_oddball;
    else if ( widget->parent->parameters.text_box_parameters.string_list_index )
        game_engine_index = game_engine_race;
    else
        profile->universal_variant.teams = 1;

    if ( game_engine_index != profile->game_engine_index )
    {
        /* zero the 6 ints of the engine-specific union following time_limit, as compiled */
        int *engine_variant_words = &profile->universal_variant.time_limit;
        for ( int i = 6; i; i-- )
            *++engine_variant_words = 0;
    }
    profile->game_engine_index = game_engine_index;
    return 1;
}
