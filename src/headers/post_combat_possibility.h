#pragma once
/* post_combat_possibility — one candidate entry in an encounter's post-combat "possibility" list: a weighted
 * actor/prop/unit triple, kept sorted descending by weight (DB types_members-confirmed, 16 bytes). */

typedef struct post_combat_possibility
{
    int   actor_index; /* 0x00 */
    float weight;       /* 0x04 */
    int   prop_index;   /* 0x08 */
    int   unit_index;   /* 0x0C */
} post_combat_possibility; /* 16 bytes */
