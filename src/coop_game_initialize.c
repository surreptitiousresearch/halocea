#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"


uint8_t coop_game_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_spawn_count = 1;
    coop_mode_selected = 1;
    return 1;
}
