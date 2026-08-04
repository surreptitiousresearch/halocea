#pragma once
/* game_engine_place — a "place" reference (goal/team home/etc.) used by game-engine UI text, passed by
 * value packed into one register (4 bytes, DB layout via types_members). */

typedef struct game_engine_place
{
    __int16 flags; /* 0x00 */
    __int16 place; /* 0x02 */
} game_engine_place;
