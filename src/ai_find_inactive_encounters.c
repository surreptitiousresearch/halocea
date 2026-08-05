/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* ai_find_inactive_encounters @0x836E7B68 — build a working list of AI entities eligible for release because
 * they have gone inactive. Walks every actor in the encounterless list, recording those that are inactive yet
 * were active at some point (last_active_time != -1), then walks every encounter recording those inactive
 * encounters that still have members and were previously active. Each 12-byte record holds { type byte (1 =
 * actor, 0 = encounter) at +0, entity index at +4, last_active_time at +8 }, packed from working_memory+4 with
 * the record count in the leading u16. The list is capped at 256 records, then sorted by
 * compare_releasable_inactive_encounters (oldest-first release ordering).
 *
 * The working buffer records are written by raw offset, matching the binary; the second u16 of the header is
 * cleared but otherwise unused. working_memory_size is not referenced (the cap is the hard-coded 256). */

#include <stdint.h>
#include "headers/ai_inactive_entity_record.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_iterator.h"
#include "headers/encounter_actor_iterator.h"

extern void encounter_iterator_new(encounter_iterator *iterator, uint8_t active_only);
extern void qsort(void *base, unsigned int count, unsigned int size,
                  int (*compare)(const void *, const void *));
extern int compare_releasable_inactive_encounters(const void *p1, const void *p2);

void ai_find_inactive_encounters(uint8_t *working_memory, int16_t working_memory_size)
{
    ai_inactive_release_memory *memory = (ai_inactive_release_memory *)working_memory;
    memory->count = 0;
    memory->cursor = 0;

    encounter_iterator iterator[2];
    /* DEVIATION: the actor loop drives the same stack slot as an encounter_actor_iterator; the prior
     * `*(int *)&iterator[0].encounter_iterator.absolute_index` was that view's `index` member (+0x4) */
    encounter_actor_iterator *actor_iterator = (encounter_actor_iterator *)iterator;

    encounter_actor_iterator_new(actor_iterator, -1);
    for ( actor_datum *actor = encounter_actor_iterator_next(actor_iterator);
          actor;
          actor = encounter_actor_iterator_next(actor_iterator) )
    {
        int16_t count = memory->count;
        if ( count >= 256 )
            break;
        if ( !actor->meta.active && actor->meta.last_active_time != -1 )
        {
            memory->records[count].is_actor = 1;
            memory->records[count].entity_index = actor_iterator->index;
            memory->records[count].last_active_time = actor->meta.last_active_time;
            memory->count = count + 1;
        }
    }

    encounter_iterator_new(iterator, 0);
    for ( encounter_datum *encounter = encounter_iterator_next(iterator);
          encounter;
          encounter = encounter_iterator_next(iterator) )
    {
        int16_t count = memory->count;
        if ( count >= 256 )
            break;
        if ( !encounter->active && encounter->current_count > 0 && encounter->last_active_time != -1 )
        {
            memory->records[count].is_actor = 0;
            memory->records[count].entity_index = iterator[0].index;
            memory->records[count].last_active_time = encounter->last_active_time;
            memory->count = count + 1;
        }
    }

    int16_t record_count = memory->count;
    if ( record_count > 0 )
        qsort(memory->records, record_count, sizeof(ai_inactive_entity_record), compare_releasable_inactive_encounters);
}
