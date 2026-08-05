#pragma once
#include <stdint.h>
/* ctf_variant — capture-the-flag game-engine variant settings (8 bytes, DB layout). */

typedef struct ctf_variant
{
    uint8_t assault;               /* 0x0 */
    uint8_t reset_on_capture;      /* 0x1 */
    uint8_t flag_must_reset;       /* 0x2 */
    uint8_t flag_at_home_to_score; /* 0x3 */
    int             single_flag_time;      /* 0x4 */
} ctf_variant;                             /* 8 bytes */
