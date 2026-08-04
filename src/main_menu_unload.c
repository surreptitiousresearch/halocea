/* main_menu_unload @0x83689478 — tear down the main menu: stop its music, mark it inactive, clear the
 * scenario-loaded flag, and disable mode-event input. */

#include <stdint.h>
#include "headers/main_globals.h"

extern void ui_stop_main_menu_music(void);
extern void main_menu_active(uint8_t active);
extern void input_abstraction_set_enabled_mode_events(uint8_t enabled);

void main_menu_unload(void)
{
    ui_stop_main_menu_music();
    main_menu_active(0);
    main_globals.main_menu_scenario_loaded = 0;
    input_abstraction_set_enabled_mode_events(0);
}
