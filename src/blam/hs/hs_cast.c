/* hs_cast @0x8368D850 — coerce a HaloScript value from actual_type to desired_type. Identity types, the
 * passthrough type (hs_passthrough), and the object-reference family (object-name types) as a destination are
 * returned unchanged. An object-name source cast to an object-type destination resolves the name index to an
 * object index; any other mismatch goes through the 2D typecasting_procedures conversion table.
 *
 * Deviation: SHIWORD(value) is the high 16 bits (the packed name index). */

#include <stdint.h>
#include "headers/hs_type.h"

#include "headers/blam_data_globals.h"   /* typecasting_procedures — canonical decl, was inline here */
extern int object_index_from_name_index(int16_t name_index);

int hs_cast(int thread_index, int16_t actual_type, int16_t desired_type, int value)
{
    if ( actual_type != desired_type && actual_type != hs_passthrough
      && (desired_type < first_hs_object_name_type || desired_type > last_hs_object_name_type) )
    {
        if ( desired_type < first_hs_object_type || desired_type > last_hs_object_type )
            return typecasting_procedures[desired_type][actual_type](value);
        else if ( actual_type >= first_hs_object_name_type && actual_type <= last_hs_object_name_type )
            return object_index_from_name_index((int16_t)(value >> 16));
    }
    return value;
}
