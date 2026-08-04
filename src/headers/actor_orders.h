#pragma once
/* actor_orders — actor_datum member at 0x3E8 (1000), 132 bytes. Full top-level layout from an
 * authoritative actor_datum structure dump (ground truth; supersedes any prior opaque placeholder).
 * Distinct from actor_external_orders (the 36-byte script-override block at 0x1C8). Sub-struct
 * internals (look/move/combat) are not yet reverse-engineered. */

#include "actor_look_orders.h"
#include "actor_move_orders.h"
#include "actor_combat_orders.h"

typedef struct actor_orders
{
    actor_look_orders   look;    /* 0x00 */
    actor_move_orders   move;    /* 0x18 */
    actor_combat_orders combat;  /* 0x6C */
} actor_orders; /* 0x84 = 132 bytes */
