#pragma once
#include <stdint.h>
/* action_specification — one entry in the global actor-action dispatch table (56 bytes, full DB layout).
 * Each actor action class supplies a set of callbacks (all taking the actor index).
 *
 * CALLBACK PARAMETER WIDTH (decided 2026-08-12, UNIT #134 stage 0 — the whole actor family pivots
 * on this): every slot's first parameter is `int actor_index`, a full 32-bit datum handle, NOT a
 * `uint16_t` absolute index. Two independent oracles:
 *
 *   1. the dispatchers tail-branch with r3 UNTOUCHED. actor_action_control @0x837F1A28 masks the
 *      handle into a SECOND register for its own subscript (`clrlwi r10,r3,16` @837F1A2C ->
 *      `mulli r9,r10,0x724` -> `lwz r11,0x34(...)`, i.e. the DATA_ARRAY_ELEMENT expansion) and then
 *      `mtctr r11 / bctr` @837F1A64-68 with r3 never rewritten. actor_action_perform @0x837F1990
 *      is the same shape (`bctr` @837F19D4). So the callee is entered with the caller's full word;
 *      nothing truncates at the boundary.
 *   2. types_members types all nine slots `void (__fastcall *)(int)` / `(int, real_argb_color *)` /
 *      `(int, int, int)`.
 *
 * DEVIATION: a handler's own opening `clrlwi rN,r3,16` is DATA_ARRAY_ELEMENT's internal truncation
 * (data_array.h), a ROLE marker, not an ABI width — reading it as one is what produced the 31
 * `uint16_t actor_index` handler declarations this unit widened. A narrow slot would zero-extend
 * the identifier salt away at every dispatch. */

/* real_argb_color is a UNION in the DB (see real_argb_color.h); the struct-tag
 * forward decl was a tag-kind mismatch clang rejects (MSVC tolerated it). */
union real_argb_color;

typedef struct action_specification
{
    int          action;                                        /* 0x00 */
    const char  *name;                                          /* 0x04 */
    const union real_argb_color **color;                        /* 0x08 */
    unsigned int data_size;                                     /* 0x0C */
    int16_t      action_class;                                  /* 0x10 */
    unsigned char _pad0[2]; /* db-verified padding */
    void (*begin)(int actor_index);                             /* 0x14 */
    uint8_t (*perform)(int actor_index);                /* 0x18 */
    void (*update)(int actor_index);                            /* 0x1C */
    void (*control)(int actor_index);                           /* 0x20 */
    void (*end)(int actor_index);                               /* 0x24 */
    void (*modify_color)(int actor_index, union real_argb_color *color); /* 0x28 */
    void (*replace_prop)(int actor_index, int a, int b);                             /* 0x2C */
    void (*flush_position_indices)(int actor_index);             /* 0x30 */
    void (*flush_structure_indices)(int actor_index); /* 0x34 */
} action_specification;                                          /* 56 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern action_specification global_action_functions[];

#ifdef __cplusplus
}
#endif
