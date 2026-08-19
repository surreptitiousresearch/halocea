/* decal_new @0x83742E78 — cast a ray from origin along velocity and, if it strikes structure (collision
 * type 2), place a decal of the given definition there. Skipped when environment decals are disabled, or
 * when the decal would be filtered out (unless it is a sequence-1 / type-3 forced decal). When a forced
 * sequence is requested the local random seed is temporarily replaced with a hash of the origin so the
 * decal's randomized sequence/orientation is deterministic for that point, then restored.
 *
 * Decal definition: flags word @0x00 (0x10 = no environment placement), type word @0x02, layer @0x04. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/decal_definition_flags.h"
#include "headers/decal_definition.h"
#include "headers/decal_layer.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/collision_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result_type.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"

#include "headers/decal_editor_geometry.h"
extern uint32_t *get_global_local_random_seed_address(void);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void decal_new_from_collision(int decal_definition_index, const collision_result *collision, const real_vector3d *velocity, float radius_modifier, uint8_t permanent, int16_t forced_sequence_index, decal_editor_geometry *editor_geometry);

void decal_new(int definition_index, const real_point3d *origin, const real_vector3d *velocity,
        float radius_modifier, uint8_t permanent, int16_t forced_sequence_index,
        decal_editor_geometry *editor_geometry)
{
    const decal_definition *definition = TAG_GET(const decal_definition, definition_index);
    int allowed = 1;
    unsigned int *seed;
    unsigned int saved_seed;
    collision_result collision;

    /* DEVIATION: the decompiler byte-tests "forced_sequence_index" here and at the two seed sites
     * below, but it mislabels the registers: it does not model f1 (radius_modifier) shadowing a GPR
     * slot, so its argument names are shifted one register left. Disasm 0x83742EC0/0x83742F0C test
     * clrlwi(r7, 24) = the uint8_t `permanent` argument (r8 = forced_sequence_index, r9 =
     * editor_geometry, both confirmed at the decal_new_from_collision call site 0x83742F98-B4). */
    if ( !rasterizer_debug_options.draw_environment_decals
            && (permanent != 1 || definition->layer != _decal_layer_alpha_tested) )  /* def+4 */
        allowed = 0;

    if ( !decals_enabled || !allowed )
        return;

    seed = get_global_local_random_seed_address();
    saved_seed = 0;
    if ( permanent )
    {
        saved_seed = *seed;
        *seed = *(const int *)&origin->n[2] ^ *(const int *)&origin->n[1]
              ^ *(const int *)&origin->n[0] ^ 0xDEADC0DE;
    }

    if ( collision_test_vector(_collision_test_environment_flags | (1u << _collision_test_try_to_keep_location_valid_bit), origin, velocity, -1, &collision)
            && collision.type == collision_result_structure
            && (definition->flags & (1u << _decal_definition_water_effect_bit)) == 0 )
    {
        decal_new_from_collision(definition_index, &collision, velocity, radius_modifier, permanent,
                forced_sequence_index, editor_geometry);
    }

    if ( permanent )
        *seed = saved_seed;
}
