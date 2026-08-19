/* close_calling_widget_if_not_editing_profile @0x836974F0 — widget callback: if a player or playlist
 * profile is currently being edited, keep the widget (return 1). Otherwise schedule the topmost parent to
 * auto-close next tick, hide it, and return 0. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"

#include "headers/player_profile.h"
#include "headers/game_variant.h"
extern player_profile * player_ui_get_edit_player_profile(void);
extern game_variant * player_ui_get_edit_playlist_profile(void);
extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);

uint8_t close_calling_widget_if_not_editing_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( player_ui_get_edit_player_profile() || player_ui_get_edit_playlist_profile() )
        return 1;
    widget_instance *topmost_parent = widget_instance_get_topmost_parent(widget);
    topmost_parent->milliseconds_to_auto_close = 1;
    topmost_parent->visible = 0;
    return 0;
}
