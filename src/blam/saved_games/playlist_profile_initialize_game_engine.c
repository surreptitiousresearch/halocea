/* playlist_profile_initialize_game_engine @0x83694798 — menu init handler: map the edited profile's
 * game-engine index onto the corresponding entry in a game-engine chooser widget and focus that
 * child. Mapping: engine 2->2, 3->3, 4->1, any other in-range engine (>=5)->4, and engine 1 or an
 * out-of-range index->0. Reports 0 when no profile is being edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int n);

uint8_t playlist_profile_initialize_game_engine(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    int16_t chooser_index;
    if ( (unsigned int)(edit_playlist_profile->game_engine_index - 1) > 4
        || edit_playlist_profile->game_engine_index == game_engine_ctf )
    {
        chooser_index = 0;
    }
    else
    {
        switch ( edit_playlist_profile->game_engine_index )
        {
            case game_engine_slayer:  chooser_index = 2; break;
            case game_engine_oddball: chooser_index = 3; break;
            case game_engine_king:    chooser_index = 1; break;
            default: chooser_index = 4; break;
        }
    }

    widget->parameters.text_box_parameters.string_list_index = chooser_index;
    widget->focused_child = widget_instance_get_nth_child(
        widget, widget->parameters.text_box_parameters.string_list_index);
    return 1;
}
