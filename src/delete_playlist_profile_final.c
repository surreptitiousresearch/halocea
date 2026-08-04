/* delete_playlist_profile_final @0x83696EA8 — widget callback: commit the pending playlist (game-settings)
 * profile deletion. Only acts on files whose saved-game type is 1 (playlist profile). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern uint16_t saved_game_file_get_type(int index);
extern void playlist_profile_delete(int playlist_profile_index);

uint8_t delete_playlist_profile_final(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    if ( saved_game_file_get_type(delete_profile_index) != 1 )
        return 0;
    playlist_profile_delete(delete_profile_index);
    return 1;
}
