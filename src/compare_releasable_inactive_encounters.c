/* compare_releasable_inactive_encounters @0x836E7B20 — qsort comparator ordering the working list built by
 * ai_find_inactive_encounters so the best release candidate sorts first. Primary key: last_active_time
 * ascending (oldest / least-recently-active first). Tiebreak: is_actor descending (actors ahead of
 * encounters when equally stale). Returns the standard -1 / 0 / +1. */

#include "headers/ai_inactive_entity_record.h"

int compare_releasable_inactive_encounters(const void *p1, const void *p2)
{
    const ai_inactive_entity_record *a = (const ai_inactive_entity_record *)p1;
    const ai_inactive_entity_record *b = (const ai_inactive_entity_record *)p2;

    int a_time = a->last_active_time;
    int b_time = b->last_active_time;
    if ( b_time < a_time )
        return 1;
    if ( b_time > a_time )
        return -1;

    unsigned int a_is_actor = a->is_actor;
    unsigned int b_is_actor = b->is_actor;
    if ( a_is_actor > b_is_actor )
        return -1;
    return a_is_actor < b_is_actor;
}
