#include "headers/ai_index_platoon_iterator.h"
#include "headers/platoon_datum.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


platoon_datum * ai_index_platoon_iterator_next(ai_index_platoon_iterator *iterator)
{
    if ( iterator->encounter_index == -1 )
        return 0;

    int platoon_index = iterator->platoon_index;
    if ( platoon_index > iterator->last_platoon_index )
        return 0;

    __int16 base_platoon_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, iterator->encounter_index)->platoon_base;

    iterator->platoon_index = platoon_index + 1;
    return &platoon_array[(__int16)(base_platoon_index + platoon_index)];
}
