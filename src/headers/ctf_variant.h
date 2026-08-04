#pragma once
/* ctf_variant — capture-the-flag game-engine variant settings (8 bytes, DB layout). */

typedef struct ctf_variant
{
    unsigned __int8 assault;               /* 0x0 */
    unsigned __int8 reset_on_capture;      /* 0x1 */
    unsigned __int8 flag_must_reset;       /* 0x2 */
    unsigned __int8 flag_at_home_to_score; /* 0x3 */
    int             single_flag_time;      /* 0x4 */
} ctf_variant;                             /* 8 bytes */
