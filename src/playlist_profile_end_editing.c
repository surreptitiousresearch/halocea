/* playlist_profile_end_editing @0x83692E58 — menu handler: clear the pending delete-profile index
 * and end the profile edit session. Always reports handled. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern void player_ui_end_editing_profile(void);

uint8_t playlist_profile_end_editing(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    delete_profile_index = -1;
    player_ui_end_editing_profile();
    return 1;
}
