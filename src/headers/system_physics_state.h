#pragma once
/* system_physics_state — per-system physics scratch state; a union of the default/explosion/jet physics
 * variants (particle_systems.c). Layout per database (4 bytes). */

#include "default_system_physics_state.h"
#include "explosion_system_physics_state.h"
#include "jet_system_physics_state.h"

typedef union system_physics_state
{
    default_system_physics_state   default_state;   /* 0x0 */
    explosion_system_physics_state explosion_state; /* 0x0 */
    jet_system_physics_state       jet_state;       /* 0x0 */
} system_physics_state;  /* 4 bytes */
