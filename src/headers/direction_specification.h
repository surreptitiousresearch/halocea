#pragma once
/* direction_specification — a tagged "look/aim toward" target (16 bytes, full DB layout). `type` selects
 * which member of the union is meaningful (prop, object, explicit point, or explicit vector). DB spells
 * the payload as anonymous-union member ___u1 of type _0CF8C33058599A7874B328E992B5BEAC ($-name kept
 * verbatim; $ in identifiers is an MSVC extension — argb_color.h convention). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

/* DB union _0CF8C33058599A7874B328E992B5BEAC — the four target shapes overlaid. */
typedef union _0CF8C33058599A7874B328E992B5BEAC
{
    int           prop_index;   /* 0x00 */
    int           object_index; /* 0x00 */
    real_point3d  point;        /* 0x00 */
    real_vector3d vector;       /* 0x00 */
} _0CF8C33058599A7874B328E992B5BEAC;

typedef struct direction_specification
{
    int16_t type;                              /* 0x00 */
    int16_t _pad02;                            /* 0x02 */
    _0CF8C33058599A7874B328E992B5BEAC ___u1;   /* 0x04 (12 bytes) */
} direction_specification;                     /* 16 bytes */
