#pragma once
/* actor_type_definition — per actor-type behavior descriptor (32 bytes, full DB layout). The runtime holds
 * an array of pointers to these (actor_type_definitions), indexed by actor meta.type. */

#include <stdint.h>
#include "real_vector3d.h"

typedef struct actor_type_definition
{
    const char     *name;                          /* 0x00 */
    int16_t         race;                           /* 0x04 */
    int16_t         when_to_search_at_target;       /* 0x06 */
    int16_t         when_to_pursue;                 /* 0x08 */
    int16_t         when_to_search_pursuit;         /* 0x0A */
    uint8_t pursuit_controller;             /* 0x0C */
    uint8_t swarm;                          /* 0x0D */
    /* 0x0E-0x0F: 2 bytes implicit padding (pointer member below forces 4-byte align to 0x10 —
     * db-verified. Left implicit so the flat static initializers map 1:1 to the named members. */
    void (*initialize)(int actor_index);                            /* 0x10 */
    void (*decide_action)(int actor_index);                         /* 0x14 */
    void (*swarm_control)(int actor_index);                         /* 0x18 */
    /* first param uint16_t 2026-07-30: matches the dispatch chain (actor_type_swarm_aim_jump ->
     * infection_swarm_aim_jump, both uint16_t actor_index — datum-index convention); was int (C4113) */
    void (*swarm_aim_jump)(uint16_t actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity); /* 0x1C */
} actor_type_definition;                            /* 32 bytes */

extern actor_type_definition *actor_type_definitions[];
