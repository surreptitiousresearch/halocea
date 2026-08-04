/* ui_widgets_disable_pause_game @0x83731220 — suppress the pause-game UI for a number of ticks. */

#include "headers/widget_globals.h"

void ui_widgets_disable_pause_game(int duration_ticks)
{
    widget_globals.pause_disabled_ticks = duration_ticks;
}
