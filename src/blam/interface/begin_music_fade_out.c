/* begin_music_fade_out @0x83697810 — widget callback: if the main-menu music is playing, stop it (begins
 * its fade-out). Always returns 1. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern uint8_t ui_main_menu_music_active(void);
extern void ui_stop_main_menu_music(void);

uint8_t begin_music_fade_out(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( ui_main_menu_music_active() )
        ui_stop_main_menu_music();
    return 1;
}
