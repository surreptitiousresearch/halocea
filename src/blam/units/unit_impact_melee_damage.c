/* unit_impact_melee_damage @0x836D0A58 — resolve a melee impact from `unit_index` onto `impact_object_index`.
 * Two outcomes, gated by the attacking unit's tag-definition unit flags:
 *   - flag 0x2000 ("melee kills self"): if the target is a live, meleeable unit (not flagged un-meleeable),
 *     deal the melee damage, deplete the attacker's body, and delete the attacker (e.g. a needle/spike that
 *     dies on contact).
 *   - flag 0xC ("impact melee attaches to unit"): if the target is a biped/vehicle (type 0 or 1) that is not
 *     dead (object.damage_flags dead bit) and the attacker is not already an ancestor in the target's parent chain, build an
 *     orthonormal orientation frame for the attacker from the (negated) impact normal — with fallbacks to the
 *     global up then global forward axis when the vectors are degenerate — reset its velocities, translate it to
 *     the impact point, attach it to the impact node, flag it (object flag 0x20, unit flag 0x8000), and begin a
 *     silent melee attack (a stuck grenade / boarding).
 *
 * The orientation frame is up = forward x (forward x up_prev); float sub-expression grouping reproduced
 * verbatim for bit-exactness. Clean decompiler output (no reg-alloc failure). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector2d.h"
extern void unit_cause_melee_damage(int unit_index, uint8_t silent, int target_object_index, int16_t target_node_index, int16_t target_region_index, int16_t target_material_index, const real_vector3d *target_object_normal);
extern void object_deplete_body(int object_index);
extern void object_delete(int object_index);
extern float normalize3d(real_vector3d *v);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index);
extern uint8_t unit_melee_attack_begin(int unit_index, uint8_t continuous, const real_vector2d *alignment_vector);

void unit_impact_melee_damage(int unit_index, int impact_object_index, int16_t impact_node_index,
        int16_t impact_region_index, int16_t impact_material_index, const real_point3d *impact_point,
        const real_vector3d *impact_normal, const location *impact_location)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_datum *impact = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, impact_object_index)->datum);

    int unit_definition_flags =
        TAG_GET(unit_definition, unit->definition_index)->unit.flags;

    if ( (unit_definition_flags & (1u << _unit_definition_impact_melee_dies_on_shields_bit)) != 0
      && !impact->object.type                                            /* target is a biped (type 0) */
      && impact->object.shield_vitality > 0.0f                           /* still alive */
      && (TAG_GET(unit_definition, impact->definition_index)->unit.flags & (1u << _unit_shields_fry_infection_forms)) != 0 )
    {
        unit_cause_melee_damage(unit_index, 1u, impact_object_index, impact_node_index, impact_region_index,
                impact_material_index, impact_normal);
        object_deplete_body(unit_index);
        object_delete(unit_index);
    }
    else if ( (unit_definition_flags & (1u << _unit_definition_impact_melee_attaches_to_unit_bit)) != 0 )
    {
        char attach = 1;
        if ( ((1 << impact->object.type) & object_mask_unit) != 0        /* target is a unit (biped/vehicle) */
          && (impact->object.damage_flags & (1u << _object_dead_bit)) == 0 )                    /* target not dead */
        {
            int ancestor = impact->object.parent_object_index;           /* walk the target's parent chain */
            if ( ancestor != -1 )
            {
                while ( 1 )
                {
                    unit_datum *ancestor_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ancestor)->datum);
                    if ( ancestor == unit_index || ancestor_object->object.type != object_type_vehicle )
                        break;                                           /* reached the attacker, or a non-vehicle */
                    ancestor = ancestor_object->object.parent_object_index;
                    if ( ancestor == -1 )
                        goto build_frame;
                }
                attach = 0;   /* attacker is an ancestor of the target: do not attach */
            }
build_frame:
            if ( attach )
            {
                unit->object.translational_velocity = *global_zero_vector3d;
                unit->object.angular_velocity = *global_zero_vector3d;
                unit->object.forward = *impact_normal;

                float forward_x = -unit->object.forward.i;
                unit->object.forward.i = forward_x;                      /* forward = -impact normal */
                float forward_y = -unit->object.forward.j;
                unit->object.forward.j = forward_y;
                float forward_z = -unit->object.forward.k;
                unit->object.forward.k = forward_z;

                /* side = forward x up_prev */
                real_vector3d side;
                side.n[0] = ((forward_z * unit->object.up.j)
                                  - (forward_y * unit->object.up.k));
                side.n[1] = (unit->object.up.k * forward_x)
                          - (unit->object.up.i * forward_z);
                side.n[2] = ((unit->object.up.i * forward_y)
                                  - (unit->object.up.j * forward_x));
                if ( normalize3d(&side) == 0.0f )
                {
                    /* forward parallel to up: use the global up axis instead */
                    side.n[0] = ((unit->object.forward.k * global_up3d->n[1])
                                      - (unit->object.forward.j * global_up3d->n[2]));
                    side.n[1] = ((global_up3d->n[2] * forward_x)
                                      - (global_up3d->n[0] * unit->object.forward.k));
                    side.n[2] = ((global_up3d->n[0] * unit->object.forward.j)
                                      - (global_up3d->n[1] * forward_x));
                    if ( normalize3d(&side) == 0.0f )
                    {
                        side.n[0] = global_forward3d->n[0];
                        side.n[1] = global_forward3d->n[1];
                        side.n[2] = global_forward3d->n[2];
                    }
                }

                /* up = forward x side */
                unit->object.up.j = (unit->object.forward.k * side.n[0])
                                  - (forward_x * side.n[2]);
                unit->object.up.k = (forward_x * side.n[1])
                                  - (unit->object.forward.j * side.n[0]);
                unit->object.up.i = (unit->object.forward.j * side.n[2])
                                  - (unit->object.forward.k * side.n[1]);

                object_translate(unit_index, impact_point, impact_location);
                object_attach_to_node(impact_object_index, unit_index, impact_node_index);
                unit->object.flags |= (1u << _object_at_rest_bit);
                unit->unit.flags |= (1u << _unit_attached_melee_attack_bit);
                unit_melee_attack_begin(unit_index, 1u, nullptr);
            }
        }
    }
}
