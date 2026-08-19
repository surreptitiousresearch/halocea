/* start_new_coop_game @0x83691FA0 — menu handler: set the cooperative flags (one spawn, coop mode)
 * and fall through to the standard new-game start. Always reports handled. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern uint8_t start_new_game(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);

uint8_t start_new_coop_game(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    player_spawn_count = 1;
    coop_mode_selected = 1;
    start_new_game(list_widget, event, widget_deleted);
    return 1;
}
