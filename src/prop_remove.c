/* prop_remove @0x837D1BF0 — unlink a prop from its actor's singly linked prop list (head at
 * meta.first_prop_index, link via prop.next_prop_index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


void prop_remove(uint16_t actor_index, int prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *props = (prop_datum *)prop_data->data;
    prop_datum *prop = &props[(unsigned __int16)actor->meta.first_prop_index];

    if ( actor->meta.first_prop_index == prop_index )
    {
        actor->meta.first_prop_index = prop->next_prop_index;
    }
    else
    {
        int next_index;
        int *link;
        do
        {
            next_index = prop->next_prop_index;
            link = &prop->next_prop_index;
            prop = &props[(unsigned __int16)next_index];
        }
        while ( next_index != prop_index );
        *link = prop->next_prop_index;
    }
}
