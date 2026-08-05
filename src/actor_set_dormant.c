/* actor_set_dormant @0x8371D528 — activate or deactivate the object(s) backing an actor as it goes dormant or
 * wakes. Only acts on an active actor whose dormant state actually changes. For a swarm actor it walks either
 * the swarm cache's unit list (swarm_data) or the inline swarm unit chain (object+508); for a normal actor it
 * acts on its single unit. Waking (dormant == 0) also clears the become-dormant timer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void object_activate(int object_index);
extern void object_deactivate(int object_index);

void actor_set_dormant(uint16_t actor_index, uint8_t dormant)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( !actor->meta.active )
        return;

    int set_dormant = dormant;  /* param now uint8_t; redundant (unsigned __int8) cast removed */
    if ( actor->meta.dormant == dormant )
        return;

    if ( actor->meta.swarm )
    {
        if ( actor->meta.swarm_cache_index == -1 )
        {
            for ( int unit_index = actor->meta.swarm_unit_index; unit_index != -1; )
            {
                unit_datum *object_data =
                    (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                if ( set_dormant )
                    object_deactivate(unit_index);
                else
                    object_activate(unit_index);
                unit_index = object_data->unit.swarm_next_unit_index;
            }
        }
        else
        {
            swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
            if ( swarm->unit_count > 0 )
            {
                for ( int16_t i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
                {
                    int unit_index = swarm->unit_indices[i];
                    if ( set_dormant )
                        object_deactivate(unit_index);
                    else
                        object_activate(unit_index);
                }
            }
        }
    }
    else
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index != -1 )
        {
            if ( dormant )
                object_deactivate(unit_index);
            else
                object_activate(unit_index);
        }
    }

    actor->meta.dormant = dormant;
    if ( !set_dormant )
        actor->meta.become_dormant_timer = 0;
}
