/* contrail_compute_new_point_count @0x8373F2A0 — how many new points to spawn for a contrail over dt.
 * Points are emitted at the definition's spacing interval (optionally density-scaled, scale_flags 0x1);
 * the contrail's time_until_point accumulator is wound down by dt, emitting a point and resetting the
 * accumulator each time it elapses. */

#include "headers/data_array.h"
#include "headers/contrail_definition_scaleable_values_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/contrail_datum.h"
#include "headers/contrail_definition.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

/* attested: dt is float (fsubs directly on f1, no frsp @0x8373F318); return int16_t
 * (callee extsh r3 @0x8373F328, caller contrail_owner_collision re-extsh) */
int16_t contrail_compute_new_point_count(int contrail_index, float dt)
{
    contrail_datum *contrail = DATA_ARRAY_ELEMENT(contrail_data, contrail_datum, contrail_index);
    contrail_definition *definition = TAG_GET(contrail_definition, contrail->definition_index);
    float spacing = definition->point_generation_rate;
    float interval;
    int16_t count = 0;

    if ( (definition->scale_flags & (1u << _contrail_point_generation_rate_bit)) != 0 )
        spacing = contrail->density * definition->point_generation_rate;   /* density-scaled */
    interval = 1.0f / spacing;

    if ( dt != 0.0f )
    {
        while ( contrail->time_until_point <= dt )
        {
            dt = dt - contrail->time_until_point;
            contrail->time_until_point = interval;
            count = (int16_t)(count + 1);
            if ( dt == 0.0f )
                return count;
        }
        contrail->time_until_point = contrail->time_until_point - dt;
    }
    return count;
}
