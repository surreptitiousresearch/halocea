#pragma once
#include <stdint.h>
/* race_globals_type — Race game-engine state (328 bytes). Full layout from DB types_members. */

typedef struct race_globals_type
{
    int           lap_completed_value;               /* 0x000 */
    int           first_flag[16];                     /* 0x004 */
    int           lap_bit_vector[16];                  /* 0x044 */
    int           rally_flag;                          /* 0x084 */
    int           team_laps[16];                       /* 0x088 */
    int           persistent_team_score[16];           /* 0x0C8 */
    int           vehicle_sound_timeout;                /* 0x108 */
    int           spawned_vehicles[8];                  /* 0x10C */
    int           number_of_spawned_vehicles;           /* 0x12C */
    int           number_of_spawned_warthogs;           /* 0x130 */
    int           number_of_spawned_ghosts;              /* 0x134 */
    int           number_of_spawned_tanks;               /* 0x138 */
    int           number_of_spawned_rocket_warthogs;      /* 0x13C */
    int           number_of_spawned_banshees;             /* 0x140 */
    uint8_t vehicles_have_been_added;              /* 0x144 */
} race_globals_type; /* 328 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern race_globals_type race_globals;

#ifdef __cplusplus
}
#endif
