#pragma once

typedef struct slayer_globals_type
{
    int team_score[16];       /* 0x00 */
    int individual_score[16]; /* 0x40 */
} slayer_globals_type;        /* 128 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern slayer_globals_type slayer_globals;

#ifdef __cplusplus
}
#endif
