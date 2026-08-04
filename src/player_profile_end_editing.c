/* player_profile_end_editing @0x836963E0 — widget callback: finish editing the current profile. If a
 * profile is being edited, re-commit controller 0's active profile (read then write it back) so the
 * edits take effect, then end the edit session. Clears the pending delete index. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/blam_data_globals.h"

extern player_profile * player_ui_get_edit_player_profile(void);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern void player_ui_end_editing_profile(void);

uint8_t player_profile_end_editing(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    player_profile active_profile;

    delete_profile_index = -1;
    if ( edit_player_profile )
    {
        int active_player_profile_index = player_ui_get_active_player_profile_index(0);
        if ( active_player_profile_index != -1 )
        {
            player_ui_get_active_player_profile(0, &active_profile);
            player_ui_set_active_player_profile(0, active_player_profile_index, &active_profile);
        }
        player_ui_end_editing_profile();
    }
    return 1;
}
