/* object_deplete_body @0x836B2C00 — mark an object's body as depleted (destroyed). Sets the dead
 * damage flag, spawns the collision model's body_depleted_effect, and — for a vehicle
 * — kills every seated occupant that isn't a deathless cheat player, then depletes the shield.
 *
 * Deviation: the effect_new_from_object call's color/impulse_field args are NULL (r9=r10=0); the
 * decompiler's non-NULL color came from the phantom GPR slot reserved by the float scale args (verified
 * at 0x836B2C78-0x836B2C90). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_vector_field.h"
#include "headers/global_tag_instances.h"
#include "headers/real_rgb_color.h"
#include "headers/cheat_globals.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void unit_kill(int unit_index);
extern void object_deplete_shield(int object_index);

void object_deplete_body(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int16_t object_flags = object->object.damage_flags;
    if ( (object_flags & (1u << _object_dead_bit)) != 0 )
        return;

    object->object.damage_flags = object_flags | (1u << _object_dead_bit);

    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    int collision_model_index = definition->collision_model.index;
    if ( collision_model_index != -1 )
    {
        collision_model *collision = TAG_GET(collision_model, collision_model_index);
        effect_new_from_object(collision->resistance.body_depleted_effect.index,
                               object_index, object_index, -1, 0.0, 0.0, 0, 0);
    }

    if ( object->object.type == object_type_vehicle )  /* vehicle */
    {
        for ( int seat = object->object.first_child_object_index; seat != -1; )
        {
            unit_datum *occupant = (unit_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat)->datum;
            if ( !occupant->object.type  /* biped */
              && (occupant->unit.player_index == -1 || !cheat.deathless_player)
              && (uint16_t)occupant->unit.parent_seat_index != 0xFFFF )
            {
                unit_kill(seat);
            }
            seat = occupant->object.next_object_index;
        }
    }

    object_deplete_shield(object_index);
}
