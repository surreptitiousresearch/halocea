#pragma once
#include <stdint.h>
/* game_engine_place — a "place" reference (goal/team home/etc.) used by game-engine UI text, passed by
 * value packed into one register (4 bytes, DB layout via types_members). */

typedef struct game_engine_place
{
    int16_t flags; /* 0x00 */
    int16_t place; /* 0x02 */
} game_engine_place;
