/* player_wants_to_join_multiplayer_game @0x836920A0 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void player_ui_clear_multiplayer_variant(void);

uint8_t player_wants_to_join_multiplayer_game(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_ui_clear_multiplayer_variant();
    return 1;
}
