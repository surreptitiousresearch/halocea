/* delete_player_profile_final @0x83696E18 — widget callback: commit the pending player-profile deletion.
 * Refuses default (0x40000000-flagged) profiles and anything that is not a player-profile file type. On a
 * real player profile, deletes it; if it was controller 0's active profile, pick a new active one. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern uint16_t saved_game_file_get_type(int index);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern void player_profile_delete(int player_profile_index);
extern void player_ui_set_an_active_profile(void);

uint8_t delete_player_profile_final(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    if ( (delete_profile_index & 0x40000000) != 0 )
        return 0;
    if ( saved_game_file_get_type(delete_profile_index) )   /* type 0 == player profile */
        return 0;

    int active_player_profile_index = player_ui_get_active_player_profile_index(0);
    player_profile_delete(delete_profile_index);
    if ( delete_profile_index == active_player_profile_index )
        player_ui_set_an_active_profile();
    return 1;
}
