/* biped_accelerate @0x837B0D40 — apply a damage impulse to a biped. No-op if the biped definition forbids
 * acceleration (unit.flags bit 0x100000). A living biped (body not depleted) only takes half the impulse;
 * a depleted ("limp body") biped first stops its limp-body physics. Adds the impulse to linear velocity,
 * wakes the biped (object flags bit 0x20 cleared, biped flags |= 3), and — for ragdoll/physics bipeds
 * (body depleted, or definition biped.flags bit 0x4/0x40) — adds a random angular kick about
 * (up x impulse). Finally, a free biped reorients its facing toward the impulse direction. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_definition_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern uint32_t *get_global_random_seed_address(void);
extern float normalize3d(real_vector3d *v);
extern float real_seed_random(uint32_t *seed);
extern void biped_stop_limp_body_physics(int biped_index);
extern void biped_snap_facing(biped_datum *biped, const biped_definition *biped_definition);

void biped_accelerate(int biped_index, real_vector3d *acceleration)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    if ( (definition->unit.flags & (1u << _unit_is_special)) != 0 )  /* unit.flags: no acceleration */
        return;

    __int16 damage_flags = biped->object.damage_flags;
    if ( (damage_flags & (1u << _object_dead_bit)) == 0 )  /* living biped: take half the impulse */
    {
        acceleration->n[0] *= 0.5f;
        acceleration->n[1] *= 0.5f;
        acceleration->n[2] *= 0.5f;
    }
    else
    {
        biped_stop_limp_body_physics(biped_index);
    }

    biped->object.translational_velocity.__s1.i += acceleration->n[0];
    biped->object.translational_velocity.__s1.j += acceleration->n[1];
    biped->object.translational_velocity.__s1.k += acceleration->n[2];
    biped->object.flags &= ~(1u << _object_at_rest_bit);
    biped->biped.flags |= 3u;

    unsigned int definition_biped_flags = definition->biped.flags;
    if ( (damage_flags & (1u << _object_dead_bit)) != 0
      || (definition_biped_flags & (1u << _biped_flying_bit)) != 0
      || (definition_biped_flags & (1u << _biped_climbs_anything_bit)) != 0 )
    {
        real_vector3d torque;  /* up x impulse */
        torque.n[0] = (global_up3d->n[1] * acceleration->n[2]) - (global_up3d->n[2] * acceleration->n[1]);
        torque.n[1] = (global_up3d->n[2] * acceleration->n[0]) - (global_up3d->n[0] * acceleration->n[2]);
        torque.n[2] = (global_up3d->n[0] * acceleration->n[1]) - (global_up3d->n[1] * acceleration->n[0]);
        normalize3d(&torque);
        float random = real_seed_random(get_global_random_seed_address());
        float magnitude = __fsqrts(acceleration->n[0] * acceleration->n[0]
                                   + (acceleration->n[2] * acceleration->n[2]
                                      + acceleration->n[1] * acceleration->n[1]));
        float spin = magnitude * random * 1.5707964f;
        biped->object.angular_velocity.__s1.i += spin * torque.n[0];
        biped->object.angular_velocity.__s1.j += torque.n[1] * spin;
        biped->object.angular_velocity.__s1.k += torque.n[2] * spin;
    }

    if ( biped->object.parent_object_index == -1 )  /* free biped: reorient facing toward impulse */
    {
        real_vector3d facing;
        facing.n[0] = acceleration->n[0];
        facing.n[1] = acceleration->n[1];
        facing.n[2] = acceleration->n[2];
        if ( normalize3d(&facing) > 0.0f )
        {
            biped->object.forward.__s1.i = facing.n[0];
            biped->object.forward.__s1.j = facing.n[1];
            biped->object.forward.__s1.k = facing.n[2];
            biped_snap_facing(biped, definition);
        }
    }
}
