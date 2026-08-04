#pragma once

typedef struct slayer_globals_type
{
    int team_score[16];       /* 0x00 */
    int individual_score[16]; /* 0x40 */
} slayer_globals_type;        /* 128 bytes */

extern slayer_globals_type slayer_globals;
