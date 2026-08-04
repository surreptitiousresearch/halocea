/* ui_widgets_inhibit_processing @0x837309F8 — set the global flag that suspends UI widget processing. */

#include <stdint.h>
#include "headers/widget_globals.h"

void ui_widgets_inhibit_processing(uint8_t inhibit)
{
    widget_globals.processing_inhibited = inhibit;
}
