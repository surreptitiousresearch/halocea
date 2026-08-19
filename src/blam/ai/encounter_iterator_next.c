/* encounter_iterator_next @0x837099A0 — advance an encounter cursor, optionally skipping inactive encounters.
 * Returns null (and does nothing) when AI is not initialized for the current map. */

#include "headers/encounter_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern void *data_iterator_next(data_iterator *iterator);

encounter_datum *encounter_iterator_next(encounter_iterator *iterator)
{
    encounter_datum *result = nullptr;

    if ( ai_globals->ai_initialized_for_map )
    {
        do
            result = data_iterator_next(&iterator->encounter_iterator);
        while ( result && iterator->active_only && !result->active );
        iterator->index = iterator->encounter_iterator.index;
    }
    return result;
}
