/* player_profile_save_changes @0x836964C8 — widget callback: play the forward feedback sound, and if the
 * edited profile is dirty, save it. If the save did not report an error, end the edit session and delete
 * the whole profile-editor widget tree, flagging the widget as deleted. Returns the save result. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"

extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern uint8_t player_ui_edit_profile_is_dirty(void);
extern uint8_t player_ui_save_profile(void);
extern void player_ui_end_editing_profile(void);
extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);
extern void ui_widget_delete(widget_instance *widget);

uint8_t player_profile_save_changes(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int save_result = 0;

    ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
    if ( player_ui_edit_profile_is_dirty() )
        save_result = player_ui_save_profile();
    if ( !(uint8_t)save_result )
    {
        player_ui_end_editing_profile();
        widget_instance *topmost_parent = widget_instance_get_topmost_parent(widget);
        ui_widget_delete(topmost_parent);
        *widget_deleted = 1;
    }
    return save_result;
}
