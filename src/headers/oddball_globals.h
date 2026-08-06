#pragma once

typedef struct oddball_globals_type
{
    int score_to_win;                    /* 0x00 */
    int team_score[16];                  /* 0x04 */
    int individual_score[16];            /* 0x44 */
    int ball_spawn_timer[16];            /* 0x84 */
    int current_ball_owner[16];          /* 0xC4 */
    int last_oddball_baseline_time[16];  /* 0x104 */
} oddball_globals_type;                  /* 324 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern oddball_globals_type oddball_globals;

#ifdef __cplusplus
}
#endif
