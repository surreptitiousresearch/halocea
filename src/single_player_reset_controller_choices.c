#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void player_ui_reset_single_player_local_player_controllers(void);

uint8_t single_player_reset_controller_choices(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_ui_reset_single_player_local_player_controllers();
    return 1;
}
