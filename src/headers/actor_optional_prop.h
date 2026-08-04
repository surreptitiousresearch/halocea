#pragma once

/* actor_optional_prop @ DB type (12 bytes) — one candidate perception prop (a unit an actor could target),
 * queued in an actor_prop_environment until promoted to an acknowledged prop. */
typedef struct actor_optional_prop
{
    int   unit_index;         /* 0x00 */
    int   prop_index;         /* 0x04 */
    float distance_squared;   /* 0x08 */
} actor_optional_prop;
