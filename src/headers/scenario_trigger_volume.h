#pragma once
/* scenario_trigger_volume — one element of scenario.trigger_volumes (96 bytes, DB layout via
 * types_members scenario_trigger_volume). `type` selects the geometry arm of the union at +0x24:
 * type 1 = oriented box (point/forward/up frame + extents), type 0 = world-aligned box (min/max
 * rectangle). Both arms overlap the same 60 bytes. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rectangle3d.h"

/* Oriented box arm (_3036A6E79EE08CC628ECDA4BF0BB4FF9): local frame + extents. */
typedef struct scenario_trigger_volume_oriented
{
    int          unused[3];   /* 0x00 (rel +0x24) */
    real_vector3d forward;    /* 0x0C (rel +0x30 / 48) */
    real_vector3d up;         /* 0x18 (rel +0x3C / 60) */
    real_point3d position;    /* 0x24 (rel +0x48 / 72) */
    real_vector3d extents;    /* 0x30 (rel +0x54 / 84) */
} scenario_trigger_volume_oriented; /* 60 bytes */

/* World-aligned box arm (_AFD978F6A9C0BC6007E7F38926441B8E): min/max rectangle. */
typedef struct scenario_trigger_volume_world_aligned
{
    int             unused[9];  /* 0x00 (rel +0x24) */
    real_rectangle3d rectangle; /* 0x24 (rel +0x48 / 72) */
} scenario_trigger_volume_world_aligned; /* 60 bytes */

typedef struct scenario_trigger_volume
{
    int16_t          type;         /* 0x00 */
    uint16_t pad;          /* 0x02 */
    char             name[32];     /* 0x04 */
    union
    {
        scenario_trigger_volume_oriented      bounding_box;              /* 0x24 */
        scenario_trigger_volume_world_aligned world_aligned_bounding_box;/* 0x24 */
    };
} scenario_trigger_volume;          /* 96 bytes */
