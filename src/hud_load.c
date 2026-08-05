/* hud_load @ 0x83708F30 — clear scripted HUD messages and print the map's loading
 * (begin) or unloading (end) caption to the first local player. */
#include <stddef.h>   /* wchar_t */
#include <stdint.h>
#include "headers/hud_globals.h"
extern void scripted_hud_messages_clear(void);
extern const wchar_t *hud_get_item_string(int index);
extern int16_t local_player_get_next(int16_t local_player_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);

void hud_load(uint8_t begin)
{
    int16_t index = begin ? hud_globals->loading_begin_index : hud_globals->loading_end_index;
    scripted_hud_messages_clear();
    if ( index != -1 )
    {
        const wchar_t *item_string = hud_get_item_string(index);
        hud_print_message(local_player_get_next(-1), item_string);
    }
}
