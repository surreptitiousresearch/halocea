/* unit_unsuspecting @0x836D22E8 — is the unit unaware of an attacker at the given point? False if the
 * unit is invalid, not a biped (object.type != object_type_biped), or its unit definition forbids assassination (unit flags
 * bit 0x10000). True if the attacker is behind the unit's look direction (positive dot of the unit's looking
 * vector with the attacker→unit vector), or the unit currently occupies a "base" (non-special) seat. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern const char *unit_get_seat_label(int object_index);

uint8_t unit_unsuspecting(int unit_index, const real_point3d *attacker)
{
    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit
      || unit->object.type   /* only bipeds can be unsuspecting */
      || (TAG_GET(unit_definition, unit->definition_index)->unit.flags & (1u << _unit_never_unsuspecting)) != 0 )
    {
        return 0;
    }

    /* dot(looking_vector, bounding_sphere_center - attacker) > 0 → attacker is behind the look direction */
    if ( (unit->unit.looking_vector.i * (unit->object.bounding_sphere_center.n[0] - attacker->n[0]))
       + (unit->unit.looking_vector.k * (unit->object.bounding_sphere_center.n[2] - attacker->n[2]))
       + (unit->unit.looking_vector.j * (unit->object.bounding_sphere_center.n[1] - attacker->n[1])) > 0.0f )
        return 1;

    const char *seat_label = unit_get_seat_label(unit_index);
    const char *base = base_seat_labels[0];
    int diff;
    do
    {
        int c = (unsigned char)*base;
        diff = c - (unsigned char)*seat_label;
        if ( c == 0 )
            break;
        ++base;
        ++seat_label;
    }
    while ( !diff );
    return !diff;
}
