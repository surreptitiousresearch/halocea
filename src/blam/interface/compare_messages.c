/* compare_messages @ 0x836A4A78 — ordering predicate for HUD message slots.
 * Sorts by descending time (most-recent first); ties broken by descending
 * item_definition_index, then by descending magic_number. Returns a signed
 * difference in the classic qsort comparator convention. */

#include "headers/hud_message.h"

int compare_messages(const hud_message *lhs, const hud_message *rhs)
{
    int time_difference = rhs->time - lhs->time;
    if ( time_difference )
        return time_difference;

    int item_difference = rhs->item_definition_index - lhs->item_definition_index;
    if ( item_difference )
        return item_difference;

    return rhs->magic_number - lhs->magic_number;
}
