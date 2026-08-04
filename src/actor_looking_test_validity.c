/* actor_looking_test_validity @0x837FAE40 — test a candidate direction against an actor's aiming and
 * looking constraints: *valid_aiming from actor_look_valid_aim_vector (definition yaw deviation float[75],
 * the actor's attempted aiming vector (input.facing_vector), target = test_vector), and *valid_looking from
 * actor_look_valid_look_vector (definition yaw deviation float[77], cone limits from the definition's
 * combat (floats 47/48, when state.mode == _actor_mode_combat) or non-combat (floats 45/46) look-angle pair run
 * through cos(), aiming vector (input.facing_vector), attempted looking vector (input.aiming_vector),
 * look vector = test_vector).
 *
 * DEVIATION (disasm-verified): the decompiler's output here is badly scrambled — an unassigned pointer
 * written through, cos() applied to register puns, and a phantom argument. The real calls are as below;
 * actor_look_valid_aim_vector's unused facing_vector slot receives the stale dword at actor+88 (the
 * variant definition tag index) left in r3 by the definition lookup — passed through as compiled. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


extern double cos(double x);
extern uint8_t actor_look_valid_aim_vector(float yaw_deviation, const real_vector3d *facing_vector, const real_vector3d *attempted_aiming_vector);
extern uint8_t actor_look_valid_look_vector(float yaw_deviation, const real_vector2d *cone_limits, const real_vector2d *aiming_vector, const real_vector2d *attempted_looking_vector, const real_vector2d *look_vector);

void actor_looking_test_validity(uint16_t actor_index, real_vector3d *test_vector, uint8_t *valid_aiming, uint8_t *valid_looking)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);

    *valid_aiming = actor_look_valid_aim_vector(definition->looking.runtime_maximum_aiming_deviation_cosine.n[0],
                                                &actor->input.facing_vector,
                                                (const real_vector3d *)test_vector);

    real_vector2d cone_limits;
    if ( actor->state.mode == _actor_mode_combat )
    {
        cone_limits.n[0] = cos(definition->looking.combat_looking_delta_angles[0]);
        cone_limits.n[1] = cos(definition->looking.combat_looking_delta_angles[1]);
    }
    else
    {
        cone_limits.n[0] = cos(definition->looking.noncombat_looking_delta_angles[0]);
        cone_limits.n[1] = cos(definition->looking.noncombat_looking_delta_angles[1]);
    }

    *valid_looking = actor_look_valid_look_vector(definition->looking.runtime_maximum_looking_deviation_cosine.n[0], &cone_limits,
                                                  (const real_vector2d *)&actor->input.facing_vector,
                                                  (const real_vector2d *)&actor->input.aiming_vector,
                                                  (const real_vector2d *)test_vector);
}
