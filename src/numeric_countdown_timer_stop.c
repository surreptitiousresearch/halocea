#include "headers/blam_data_globals.h"
/* numeric_countdown_timer_stop @0x83755EB8 — clear the global flag driving the numeric countdown timer. */


void numeric_countdown_timer_stop(void)
{
    numeric_countdown_timer_on = 0;
}
