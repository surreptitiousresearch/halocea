/* solo_level_dispose_list @0x83691CB0 — single-player level list teardown: zero the level data
 * table, detach the widget item array (owned by the table), clear the count, and free the preview
 * list. Always reports handled. */

#include <stdint.h>
#include <string.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern void preview_list_free(void);

uint8_t solo_level_dispose_list(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    memset(single_player_level_data, 0, sizeof(single_player_level_data));
    list_widget->parameters.list_parameters.list_items = nullptr;
    list_widget->parameters.list_parameters.number_of_items = 0;
    preview_list_free();
    return 1;
}
