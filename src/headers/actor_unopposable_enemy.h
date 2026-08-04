#pragma once
/* actor_unopposable_enemy — one scratch record in the per-call working array built by
 * actor_emotion_get_unopposable_enemy()/actor_emotion_unopposable_retreat(): tracks, for one enemy unit,
 * the highest "unopposed" threat tier seen this pass and, when the enemy is currently unopposed, how many
 * nearby allies are already retreating from it plus the nearest such retreating friend. Field names and
 * layout are verbatim from the DB type actor_unopposable_enemy (28 bytes). */

#include "prop_datum.h"

typedef struct actor_unopposable_enemy
{
    __int16     unopposable_type;       /* 0x00 — 0 (init); threat tier 0..9 raised through the pass */
    char        _pad02[2];              /* 0x02 */
    int         prop_index;             /* 0x04 — -1 (init); a prop index representing "our side" against
                                         *  this enemy unit */
    int         unit_index;             /* 0x08 — the enemy unit this record tracks (search/create key) */
    prop_datum *prop;                   /* 0x0C — nullptr (init); nearest prop seen carrying this threat */
    __int16     friends_retreating;     /* 0x10 — 0 (init); count of nearby allies already retreating */
    char        _pad12[2];              /* 0x12 */
    float       closest_retreating_friend_dist_sq; /* 0x14 — FLT_MAX (init) */
    int         closest_retreating_friend_actor_index; /* 0x18 — -1 (init) */
} actor_unopposable_enemy; /* 28 bytes */
