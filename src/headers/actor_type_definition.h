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
    /* DEVIATION (2026-08-12, UNIT #134 stage 0): first param is `int actor_index`. This REFUTES the
     * 2026-08-07 note that stood here, which narrowed the slot to uint16_t on the strength of
     * `clrlwi r11, r3, 16` at the head of infection_swarm_aim_jump@0x83828848. That mask is
     * DATA_ARRAY_ELEMENT's own truncation (data_array.h) — a ROLE marker for "this is a datum
     * subscript", never an ABI width; the accessor truncates internally precisely so a full handle
     * is safe to pass. The dispatcher settles it: actor_type_swarm_aim_jump @0x837F8FC8 masks into
     * r10 for its own actor_data subscript (`clrlwi r10,r3,16` @837F8FCC -> `mulli r10,r10,0x724`)
     * and then `mtctr r11 / bctr` @837F9004-08 with r3 NEVER rewritten — the handler is entered with
     * the caller's full 32-bit word. types_members agrees
     * (`void (__fastcall *)(int, int, float, real_vector3d *)`), as does funcs.prototype for the
     * only implementation. A narrow slot zero-extends the identifier salt away at every dispatch. */
    void (*swarm_aim_jump)(int actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity); /* 0x1C */
} actor_type_definition;                            /* 32 bytes */

extern actor_type_definition *actor_type_definitions[];
