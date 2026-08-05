#pragma once
#include <stdint.h>
/* actor_target_data — actor_datum member at 0x268 (616), 24 bytes. Full layout from an authoritative
 * actor_datum structure dump (ground truth; supersedes any prior opaque placeholder). */

typedef struct actor_target_data
{
    int16_t          target_type;                     /* 0x00 */
    int16_t          artificial_target_knowledge;      /* 0x02 */
    int              target_last_visible_time;          /* 0x04 */
    int              target_prop_index;                 /* 0x08 */
    uint8_t  any_target_ever;                    /* 0x0C */
    unsigned char    _pad0D[3];                          /* 0x0D */
    int              since_any_target_visible_timer;      /* 0x10 */
    uint8_t  target_really_alive;                 /* 0x14 */
    unsigned char    _pad15[3];                            /* 0x15 */
} actor_target_data; /* 0x18 = 24 bytes */
