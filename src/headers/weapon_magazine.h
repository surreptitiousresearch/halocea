#pragma once
#include <stdint.h>
/* weapon_magazine — runtime state of one weapon magazine, embedded as weapon_datum.weapon.magazines[2].
 * Layout DB-verified against type 'weapon_magazine' (12 bytes, 2026-07-14). */

typedef struct weapon_magazine
{
    int16_t state;                       /* 0x00 */
    int16_t state_timer;                 /* 0x02 */
    int16_t original_time;               /* 0x04 */
    int16_t rounds_total;                /* 0x06 */
    int16_t rounds_loaded;               /* 0x08 */
    int16_t rounds_fractional_recharged; /* 0x0A */
} weapon_magazine;                       /* 12 bytes */
