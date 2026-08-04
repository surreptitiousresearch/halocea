#include <stdint.h>
#include "headers/blam_data_globals.h"
/* ui_loading_set_state @0x83764A88 — set the loading-screen state; a zero state requests the loading
 * screen begin closing rather than latching state 0. */

extern void ui_loading_start_closing(uint8_t why_are_we_always_preparing_just_go);

void ui_loading_set_state(int state)
{
    if ( state )
        loading_screen_state = state;
    else
        ui_loading_start_closing(1);
}
