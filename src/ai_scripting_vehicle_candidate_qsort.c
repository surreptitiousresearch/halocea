/* ai_scripting_vehicle_candidate_qsort @0x83770CF8 — qsort comparator ordering scripted "go to vehicle"
 * candidates: actors not already heading to a vehicle sort before those that are, and within each group by
 * ascending squared distance to the target vehicle. */

#include "headers/vehicle_scripting_candidate.h"

int ai_scripting_vehicle_candidate_qsort(const void *a, const void *b)
{
    const vehicle_scripting_candidate *p1 = (const vehicle_scripting_candidate *)a;
    const vehicle_scripting_candidate *p2 = (const vehicle_scripting_candidate *)b;

    if ( p1->already_going_to_vehicle != p2->already_going_to_vehicle )
        return p1->already_going_to_vehicle == 0 ? -1 : 1;
    if ( p1->distance_squared >= p2->distance_squared )
        return p1->distance_squared > p2->distance_squared;
    return -1;
}
