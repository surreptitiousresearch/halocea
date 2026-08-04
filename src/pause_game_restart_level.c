#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void main_reset_map(void);

uint8_t pause_game_restart_level(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    main_reset_map();
    return 1;
}
