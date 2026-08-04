#include <stdint.h>
#include "headers/blam_data_globals.h"
/* numeric_countdown_timer_set @0x83755C50 — set the numeric countdown timer's duration and optionally start
 * it running. */


void numeric_countdown_timer_set(int milliseconds, uint8_t auto_start)
{
    numeric_countdown_timer_milliseconds = milliseconds;
    numeric_countdown_timer_on = auto_start;
}
