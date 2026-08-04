/* difficulty_set @0x83691E58 — difficulty-list accept handler: read the selected index from the
 * list widget two parents up; if it names a valid difficulty (< 4) commit it and play the accept
 * sound. Always reports the event handled. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/ui_audio_feedback_sound.h"

extern void main_set_difficulty(int16_t difficulty);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t difficulty_set(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    __int16 selected_list_item_index =
        list_widget->parent->parent->parameters.list_parameters.selected_list_item_index;

    if ( selected_list_item_index < 4 )
    {
        main_set_difficulty(selected_list_item_index);
        ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
    }
    return 1;
}
