/* ai_scripting_braindead_by_unit @0x83771060 — script command: make brain-dead (or revive) the actors driving a
 * list of unit objects. For each object in the list that verifies as a unit (type flags 3), its actor is set
 * brain-dead; then its child objects are walked (first child at object+280, next sibling at child+276), and any
 * child whose object type is a unit (type at child+180, mask (1<<type)&3) also has its actor set brain-dead.
 *
 * An object's actor index is read at +500, falling back to +504 if the first is -1. Child object data blocks come
 * from the object datum array (12-byte elements; data pointer at +8). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void actor_braindead(int actor_index, uint8_t braindead);

void ai_scripting_braindead_by_unit(int object_list_index, uint8_t braindead)
{
    int reference_index;

    for ( int unit_index = object_list_get_first(object_list_index, &reference_index);
          unit_index != -1;
          unit_index = object_list_get_next(object_list_index, &reference_index) )
    {
        unit_datum *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
        if ( !unit_object )
            continue;

        int actor_index = unit_object->unit.actor_index;
        if ( actor_index != -1 || (actor_index = unit_object->unit.swarm_actor_index) != -1 )
            actor_braindead(actor_index, braindead);

        for ( int child = unit_object->object.first_child_object_index; child != -1; )
        {
            unit_datum *child_object =
                ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum);
            if ( ((1 << child_object->object.type) & object_mask_unit) != 0 )              /* child type is a unit */
            {
                int child_actor = child_object->unit.actor_index;
                if ( child_actor != -1 || (child_actor = child_object->unit.swarm_actor_index) != -1 )
                    actor_braindead(child_actor, braindead);
            }
            child = child_object->object.next_object_index;                /* next sibling */
        }
    }
}
