#include <stdint.h>
#include "headers/encounter_iterator.h"
#include "headers/ai_globals.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void data_iterator_new(data_iterator *iterator, data_array *data);

void encounter_iterator_new(encounter_iterator *iterator, uint8_t active_only)
{
    if (ai_globals->ai_initialized_for_map)
    {
        data_iterator_new(&iterator->encounter_iterator, encounter_data);
        iterator->active_only = active_only;
    }
}
