/* multiplayer_edit_profile_set_ruleset_textbox_string_index @0x83780058 — sets a textbox widget's string
 * list index to the ruleset name matching the currently-edited playlist profile's game engine (falls back
 * to a generic index if there's no profile being edited, or the engine index is out of the known range). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_game_text_string.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);

void multiplayer_edit_profile_set_ruleset_textbox_string_index(widget_instance *widget)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return;

    int16_t string_list_index;
    if ( (unsigned int)(edit_playlist_profile->game_engine_index - 1) > 4 )
    {
        string_list_index = _string_unknown_game_type;
    }
    else switch ( edit_playlist_profile->game_engine_index )
    {
        case game_engine_ctf: string_list_index = _string_capture_the_flag; break;
        case game_engine_slayer: string_list_index = _string_slayer; break;
        case game_engine_oddball: string_list_index = _string_oddball; break;
        case game_engine_king: string_list_index = _string_king_of_the_hill; break;
        default: /* game_engine_race */ string_list_index = _string_race; break;
    }

    widget->parameters.text_box_parameters.string_list_index = string_list_index;
}
