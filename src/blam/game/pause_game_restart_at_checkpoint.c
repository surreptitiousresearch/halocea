/* pause_game_restart_at_checkpoint @0x83691FE0 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void main_revert_map(void);

uint8_t pause_game_restart_at_checkpoint(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    main_revert_map();
    return 1;
}
