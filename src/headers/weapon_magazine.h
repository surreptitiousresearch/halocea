#pragma once
/* weapon_magazine — runtime state of one weapon magazine, embedded as weapon_datum.weapon.magazines[2].
 * Layout DB-verified against type 'weapon_magazine' (12 bytes, 2026-07-14). */

typedef struct weapon_magazine
{
    __int16 state;                       /* 0x00 */
    __int16 state_timer;                 /* 0x02 */
    __int16 original_time;               /* 0x04 */
    __int16 rounds_total;                /* 0x06 */
    __int16 rounds_loaded;               /* 0x08 */
    __int16 rounds_fractional_recharged; /* 0x0A */
} weapon_magazine;                       /* 12 bytes */
