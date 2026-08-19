#include <stdint.h>
#include "headers/blam_data_globals.h"
/* numeric_countdown_timer_get @0x83755C68 — extract a single display field from the global numeric
 * countdown timer (a millisecond count). digit_index -1 returns the raw millisecond value (as a signed
 * 16-bit); 0..8 return individual clock digits: 0=centiseconds ones, 1=tens, 2=deciseconds, 3=seconds
 * ones, 4=tens-of-seconds (0..5), 5=minutes ones, 6=tens-of-minutes (0..5), 7=hours ones, 8=tens-of-hours. */


int16_t numeric_countdown_timer_get(int16_t digit_index)
{
    switch (digit_index)
    {
        case -1: return (int16_t)numeric_countdown_timer_milliseconds;
        case 0:  return numeric_countdown_timer_milliseconds % 10;
        case 1:  return numeric_countdown_timer_milliseconds / 10 % 10;
        case 2:  return numeric_countdown_timer_milliseconds / 100 % 10;
        case 3:  return numeric_countdown_timer_milliseconds / 1000 % 10;
        case 4:  return numeric_countdown_timer_milliseconds / 10000 % 6;
        case 5:  return numeric_countdown_timer_milliseconds / 60000 % 10;
        case 6:  return numeric_countdown_timer_milliseconds / 600000 % 6;
        case 7:  return numeric_countdown_timer_milliseconds / 3600000 % 10;
        case 8:  return numeric_countdown_timer_milliseconds / 36000000 % 10;
    }
    return 0;
}
