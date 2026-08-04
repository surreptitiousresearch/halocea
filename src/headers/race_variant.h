#pragma once
/* race_variant — race game-engine variant settings (8 bytes, DB layout). */

#include "race_type.h"

typedef struct race_variant
{
    race_type         race_type;     /* 0x00 */
    race_team_scoring team_scoring;  /* 0x04 */
} race_variant; /* 8 bytes */
