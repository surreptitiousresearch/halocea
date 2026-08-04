#pragma once
/* unit_globals @ unit_globals — per-frame unit update timer accounting (units.c). */

/* Shadow pattern: object name == type name → bare struct TAG only (no typedef). */
struct unit_globals
{
    __int16         next_timer;     /* 0x00 */
    __int16         highest_timer;  /* 0x02 */
    unsigned __int8 used_time;      /* 0x04 */
    char            pad[3];         /* 0x05 */
};                                  /* 8 bytes */

extern struct unit_globals *unit_globals;
