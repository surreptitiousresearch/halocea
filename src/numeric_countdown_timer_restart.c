#include "headers/blam_data_globals.h"
/* numeric_countdown_timer_restart @0x83755EC8 — set the global flag driving the numeric countdown timer. */


void numeric_countdown_timer_restart(void)
{
    numeric_countdown_timer_on = 1;
}
