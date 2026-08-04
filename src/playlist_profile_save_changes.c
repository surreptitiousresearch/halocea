/* playlist_profile_save_changes @0x836960E8 — menu handler: if the edited profile is dirty, either
 * prompt for a rename (default profile whose name is unchanged) or save it (returning the save
 * result). If the profile is not dirty, end the edit session, delete the topmost dialog, and flag
 * the widget as deleted. Returns the save result (0 unless a dirty save happened). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern uint8_t player_ui_edit_profile_is_dirty(void);
extern int player_ui_save_profile(void);
extern void player_ui_end_editing_profile(void);
extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);
extern void ui_widget_delete(widget_instance *widget);
/* frontier externs (deeper UI queries, not yet re-sourced) */
extern uint8_t player_ui_edit_profile_is_default_profile(void);
extern uint8_t player_ui_edit_profile_name_is_dirty(void);
extern int player_ui_prompt_user_to_rename_edit_profile(void);

uint8_t playlist_profile_save_changes(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int save_result = 0;
    char should_close = 0;

    if ( player_ui_edit_profile_is_dirty() )
    {
        if ( player_ui_edit_profile_is_default_profile() && !player_ui_edit_profile_name_is_dirty() )
            player_ui_prompt_user_to_rename_edit_profile();
        else
            save_result = player_ui_save_profile();
    }
    else
    {
        should_close = 1;
    }

    if ( should_close == 1 )
    {
        player_ui_end_editing_profile();
        ui_widget_delete(widget_instance_get_topmost_parent(widget));
        *widget_deleted = 1;
    }

    return save_result;
}
