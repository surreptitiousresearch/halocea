/* switch_from_main_menu_to_single_player @0x83696C90 — widget callback: unless a co-op mode is selected,
 * drop to a local (no network) connection, switch the main menu into single-player, and remember player 1's
 * profile without writing to disk. Returns 0 while co-op is selected, 1 otherwise. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern void game_connection_set(int16_t new_connection);
extern void main_menu_switch_to_single_player(void);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);

uint8_t switch_from_main_menu_to_single_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( coop_mode_selected )
        return 0;
    game_connection_set(0);
    main_menu_switch_to_single_player();
    player_ui_remember_player1_profile(0);
    return 1;
}
