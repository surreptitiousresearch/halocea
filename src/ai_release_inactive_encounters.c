/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* ai_release_inactive_encounters @0x836E7D30 — process the next entry from a work-queue of inactive
 * encounters/encounterless-actors to release (12-byte records at working_memory+2: count at +0, current
 * index at +2, then per-record: a type byte at +4, the actor/encounter index at +8). If the current
 * record's type byte is set it's an encounterless actor: describes and erases it. Otherwise it's a whole
 * encounter: describes it (with its live unit count) and erases it. Advances the cursor, writes a
 * human-readable description of what was released into result_description, and reports via
 * *more_to_release whether the cursor has now reached (or passed) the total count. Returns 1 if a record
 * was processed this call, 0 if the cursor was already at/past the count.
 *
 * The work-queue layout is the reconstructed (no DB/PDB type) ai_inactive_release_memory /
 * ai_inactive_entity_record — see headers/ai_inactive_entity_record.h. */

#include <stdint.h>
#include "headers/encounter_definition.h"
#include "headers/encounter_datum.h"
#include "headers/actor_datum.h"
#include "headers/ai_inactive_entity_record.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


extern char *tag_get_name(int16_t tag_index);
extern const char *tag_name_strip_path(const char *name);
extern int sprintf_0(char *string, const char *format, ...);
extern void actor_erase(int actor_index, uint8_t immediate);
extern void ai_erase(int encounter_index, int platoon_index, int squad_index, uint8_t immediate);

int ai_release_inactive_encounters(char *result_description, uint8_t *more_to_release, uint8_t *working_memory, int16_t working_memory_size)
{
    ai_inactive_release_memory *memory = (ai_inactive_release_memory *)working_memory;
    int16_t cursor = memory->cursor;
    int result = 0;

    if ( cursor < memory->count )
    {
        ai_inactive_entity_record *record = &memory->records[cursor];
        int index = record->entity_index;

        if ( record->is_actor )
        {
            const char *name = tag_get_name(DATUM_GET(actor_data, actor_datum, index)->meta.variant_definition_index);
            const char *stripped_name = tag_name_strip_path(name);
            sprintf_0(result_description, "encounterless-actor %s", stripped_name);
            actor_erase(index, 1u);
        }
        else
        {
            sprintf_0(result_description, "encounter %s (%d units)",
                ((encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)index)->name,
                DATUM_GET(encounter_data, encounter_datum, index)->current_count);
            ai_erase(index, -1, -1, 1u);
        }

        result = 1;
        ++memory->cursor;
    }

    int16_t count = memory->count;
    int16_t new_cursor = memory->cursor;
    *more_to_release = (((count ^ new_cursor) >= 0) + (new_cursor >= (unsigned int)count)) & 1;

    return result;
}
