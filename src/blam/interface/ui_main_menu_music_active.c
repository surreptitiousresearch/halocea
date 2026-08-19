/* ui_main_menu_music_active @0x83731210 — return whether the main-menu music is currently active. */

#include <stdint.h>
#include "headers/widget_globals.h"

uint8_t ui_main_menu_music_active(void)
{
    return widget_globals.main_menu_music_active;
}
