#pragma once
#include <stdint.h>
/* action_specification — one entry in the global actor-action dispatch table (56 bytes, full DB layout).
 * Each actor action class supplies a set of callbacks (all taking the actor index). */

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
    void (__fastcall *begin)(int actor_index);                  /* 0x14 */
    uint8_t (__fastcall *perform)(int actor_index);     /* 0x18 */
    void (__fastcall *update)(int actor_index);                 /* 0x1C */
    void (__fastcall *control)(int actor_index);                /* 0x20 */
    void (__fastcall *end)(int actor_index);                    /* 0x24 */
    void (__fastcall *modify_color)(int actor_index, union real_argb_color *color); /* 0x28 */
    void (__fastcall *replace_prop)(int actor_index, int a, int b);                  /* 0x2C */
    void (__fastcall *flush_position_indices)(int actor_index);  /* 0x30 */
    void (__fastcall *flush_structure_indices)(int actor_index); /* 0x34 */
} action_specification;                                          /* 56 bytes */

extern action_specification global_action_functions[];
