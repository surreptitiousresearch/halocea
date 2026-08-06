/* hs_can_cast @0x8368D718 — decide whether a value of `actual_type` can satisfy a slot wanting `desired_type`.
 * The pass-through type (hs_passthrough) and identical types always cast. Object types come in two banks
 * (first_hs_object_type..last_hs_object_type = object designators, first_hs_object_name_type..
 * last_hs_object_name_type = object references); within/across the banks compatibility is a bitmask
 * containment test via hs_object_type_masks[]. All other type pairs are decided by the presence of a
 * procedure in the typecasting_procedures[desired][actual] table. */

#include <stdint.h>
#include "headers/hs_type.h"

/* Both globals come from the canonical header rather than a local extern. This file previously
 * spelled the table `typecasting_procedure typecasting_procedures[][49]` over a local
 * `typedef void *typecasting_procedure` — same 4-byte element and the same index math, but an
 * incomplete outer dimension and an element type that discards the call signature. The definition
 * (src/data/typecasting_procedures.c) and hs_cast.c both use `int (*[49][49])(int)`, which is what
 * `applied_types` states; three spellings of one object is exactly the divergence the canonical
 * header exists to make impossible. */
#include "headers/blam_data_globals.h"

/* params extsh r3/r4 -> signed int16_t; return byte-normalized (all 5 callers clrlwi r3,24) -> uint8_t */
uint8_t hs_can_cast(int16_t actual_type, int16_t desired_type)
{
    if ( actual_type == hs_passthrough || actual_type == desired_type )
        return 1;

    if ( desired_type >= first_hs_object_type && desired_type <= last_hs_object_type )
    {
        if ( actual_type >= first_hs_object_type && actual_type <= last_hs_object_type )
        {
            int16_t m = hs_object_type_masks[actual_type - first_hs_object_type];
            return m == (hs_object_type_masks[desired_type - first_hs_object_type] & m);
        }
        if ( actual_type >= first_hs_object_name_type && actual_type <= last_hs_object_name_type )
        {
            int16_t m = hs_object_type_masks[actual_type - first_hs_object_name_type];
            return m == (hs_object_type_masks[desired_type - first_hs_object_type] & m);
        }
        return 0;
    }

    if ( desired_type < first_hs_object_name_type || desired_type > last_hs_object_name_type )
        return typecasting_procedures[desired_type][actual_type] != nullptr;

    if ( actual_type < first_hs_object_name_type || actual_type > last_hs_object_name_type )
        return 0;
    int16_t m = hs_object_type_masks[actual_type - first_hs_object_name_type];
    return m == (hs_object_type_masks[desired_type - first_hs_object_name_type] & m);
}
