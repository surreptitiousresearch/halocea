#pragma once
/* scenario_object_permutation — per-placement region/color customization (32 bytes, DB
 * types_members-confirmed layout). Originally left opaque when scenery_place's sole use was pointer
 * arithmetic; filled in once biped_place/vehicle_place needed to embed it by value. */

typedef struct scenario_object_permutation
{
    unsigned int  change_colors[4];       /* 0x00 */
    unsigned char region_permutations[8]; /* 0x10 */
    unsigned int  unused2[2];             /* 0x18 */
} scenario_object_permutation; /* 32 bytes */
