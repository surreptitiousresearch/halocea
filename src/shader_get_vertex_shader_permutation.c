/* shader_get_vertex_shader_permutation 0x83755638 — select the vertex-shader permutation index for a
 * shader based on its class (base.type) and fields of the *concrete* shader tag. Returns 0 for an
 * invalid/null shader or a class that has no permutations.
 *
 * DEVIATION: the decompiler only knew the 40-byte `shader` base type, so every read of a concrete-tag
 * field came out as a subscript on that base (`shader[N].base.radiosity.*`, N = byte_offset / 40). Each
 * one is really a member of the derived tag body, which begins at +0x28. Re-derived from the loads:
 *   0x58 `lwz` @0x8375569C  -> shader_effect.effect.secondary_map.index
 *   0x5C `lhz` @0x837556A8  -> shader_effect.effect.secondary_map_anchor
 *   0x38 `lfs` @0x83755678  -> shader_model.model.translucency
 *   0x2A `lhz` @0x837556BC/0x83755700/0x83755744 -> <generic|chicago|chicago_extended>.type
 *   0x29 `lbz` @0x837556D0/0x83755714/0x83755758 -> <generic|chicago|chicago_extended>.flags (a BYTE
 *        at +0x01 of the derived body, which is why the folded halfword read still landed on it)
 *   0x00 `lhz` @0x837556E4  -> the genuine base member shader->base.radiosity.flags */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_radiosity_flags.h"
#include "headers/shader_effect.h"
#include "headers/shader_model.h"
#include "headers/shader_transparent_generic.h"
#include "headers/shader_transparent_generic_flags.h"
#include "headers/shader_transparent_chicago.h"
#include "headers/shader_transparent_chicago_extended.h"
#include "headers/shader_transparent_chicago_flags.h"

int16_t shader_get_vertex_shader_permutation(const shader *shader)
{
    int permutation;

    /* DEVIATION: unsigned range test (type - 1 > 6) unfolded — permutations exist only for
     * shader types effect..transparent_chicago_extended */
    if ( shader == (const struct shader *)-1
      || shader->base.type < _shader_type_effect
      || shader->base.type > _shader_type_transparent_chicago_extended )
        return 0;

    switch ( shader->base.type )
    {
        case _shader_type_effect:
        {
            const shader_effect *effect_shader = (const shader_effect *)shader;
            if ( effect_shader->effect.secondary_map.index != -1 )
                return (int16_t)(effect_shader->effect.secondary_map_anchor + 1);
            return 0;
        }

        case _shader_type_decal:
        case _shader_type_environment:
            return 0;

        case _shader_type_model:
            return ((const shader_model *)shader)->model.translucency > 0.0f;

        case _shader_type_transparent_generic:
        {
            /* permutation = first-map-type + 1; permutation 1 is _..._type_2d_map, which further
             * degrades to 0 unless the first map is drawn in screenspace */
            const shader_transparent_generic *generic_shader = (const shader_transparent_generic *)shader;
            permutation = (int16_t)(generic_shader->generic.type + 1);
            if ( permutation == 1 )
                permutation = (generic_shader->generic.flags
                        & (1u << _shader_transparent_generic_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;
        }

        case _shader_type_transparent_chicago:
        {
            const shader_transparent_chicago *chicago_shader = (const shader_transparent_chicago *)shader;
            permutation = (int16_t)(chicago_shader->chicago.type + 1);
            if ( permutation == 1 )
                permutation = (chicago_shader->chicago.flags
                        & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;
        }

        default:
        {
            /* chicago_extended shares the chicago flags/type layout (no separate DB enum exists) */
            const shader_transparent_chicago_extended *extended_shader =
                    (const shader_transparent_chicago_extended *)shader;
            permutation = (int16_t)(extended_shader->chicago_extended.type + 1);
            if ( permutation == 1 )
                permutation = (extended_shader->chicago_extended.flags
                        & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;
        }
    }
    return permutation;
}
