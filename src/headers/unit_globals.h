#pragma once
#include <stdint.h>
/* unit_globals @ unit_globals — per-frame unit update timer accounting (units.c). */

/* Shadow pattern: object name == type name → bare struct TAG only (no typedef). */
struct unit_globals
{
    int16_t         next_timer;     /* 0x00 */
    int16_t         highest_timer;  /* 0x02 */
    uint8_t used_time;      /* 0x04 */
    char            pad[3];         /* 0x05 */
};                                  /* 8 bytes */

extern struct unit_globals *unit_globals;
