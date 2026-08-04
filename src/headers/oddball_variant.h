#pragma once
/* oddball_variant — oddball game-engine variant settings (24 bytes, DB layout). */

#include "oddball_speed.h"
#include "game_engine_trait.h"
#include "oddball_ball_type.h"

typedef struct oddball_variant
{
    unsigned __int8   random_start;       /* 0x00 */
    unsigned __int8   ball_spawn_delay;   /* 0x01 */
    unsigned __int8   _pad02[2];          /* 0x02 */
    oddball_speed     speed_with_ball;    /* 0x04 */
    game_engine_trait trait_with_ball;    /* 0x08 */
    game_engine_trait trait_without_ball; /* 0x0C */
    oddball_ball_type oddball_ball_type;  /* 0x10 */
    int               ball_spawn_count;   /* 0x14 */
} oddball_variant;                        /* 24 bytes */
