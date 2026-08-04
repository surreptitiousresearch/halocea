/* video_screen_init @0x83785248 — video settings screen "open" handler: if the video test hasn't been
 * confirmed yet, just report whether a player profile is being edited; otherwise consume the confirmation
 * and auto-close the topmost dialog immediately. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/blam_data_globals.h"

#include "headers/player_profile.h"
extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);
extern player_profile * player_ui_get_edit_player_profile(void);

uint8_t video_screen_init(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    video_settings_saved = 0;
    if ( !video_test_save_confirmed )
        return player_ui_get_edit_player_profile() != 0;

    widget_instance *topmost_parent = widget_instance_get_topmost_parent(widget);

    video_test_save_confirmed = 0;
    topmost_parent->milliseconds_to_auto_close = 1;
    topmost_parent->auto_close_fade_time = 0;
    topmost_parent->visible = 0;
    return 1;
}
