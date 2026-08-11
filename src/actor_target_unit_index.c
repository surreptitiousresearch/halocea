#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


int actor_target_unit_index(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int prop_index = actor->target.target_prop_index;

    if (prop_index == -1)
        return -1;

    return (DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index))->unit_index;
}
