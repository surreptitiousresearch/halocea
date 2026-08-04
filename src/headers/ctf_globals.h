#pragma once
#include "data_array.h"
#include "scenario_netgame_flag.h"

typedef struct ctf_globals_type
{
    scenario_netgame_flag  *flags[2];                           /* 0x00 */
    int                     weapon_index[2];                    /* 0x08 */
    int                     score[2];                           /* 0x10 */
    int                     score_to_win;                       /* 0x18 */
    unsigned __int8         flag_warning[2];                    /* 0x1C */
    unsigned __int8         _pad1E[2];
    int                     flag_warning_time_in_ticks[2];      /* 0x20 */
    int                     flag_swap_timer;                    /* 0x28 */
    int                     non_deterministic_next_flag_failure_time; /* 0x2C */
    unsigned __int8         team_with_flag;                     /* 0x30 — DB layout (single-flag CTF) */
    unsigned __int8         _pad31[3];
} ctf_globals_type;

extern ctf_globals_type ctf_globals;
