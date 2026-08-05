/* shader_get_vertex_shader_permutation 0x83755638 — select the vertex-shader permutation index for a
 * shader based on its class (base.type) and the concrete-subtype fields that overlap the generic base
 * shader. Returns 0 for an invalid/null shader or a class that has no permutations.
 *
 * DEVIATION: the decompiler expresses the subtype field reads by punning through the 40-byte `shader`
 * base array (shader[N].base.radiosity.*); the LODWORD/HIWORD are bit reinterpretations of the float
 * radiosity slots, which the concrete shader subtype actually stores int16 indices in. Written as explicit
 * big-endian-correct bit extraction: *(int*)&f (whole 32-bit slot) and (u16)(*(u32*)&f >> 16) (high word,
 * which on big-endian PowerPC is the first int16 in memory). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"
#include "headers/shader_radiosity_flags.h"
#include "headers/shader_transparent_generic_flags.h"
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
            if ( *(int *)&shader[2].base.radiosity.color.n[0] != -1 )
                return (int16_t)((uint16_t)(*(unsigned int *)&shader[2].base.radiosity.color.green >> 16) + 1);
            return 0;

        case _shader_type_decal:
        case _shader_type_environment:
            return 0;

        case _shader_type_model:
            return shader[1].base.radiosity.color.n[2] > 0.0;

        case _shader_type_transparent_generic:
            permutation = (int16_t)(shader[1].base.radiosity.detail_level + 1);
            if ( permutation == 1 )
                permutation = (shader[1].base.radiosity.flags & (1u << _shader_transparent_generic_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;

        case _shader_type_transparent_chicago:
            permutation = (int16_t)(shader[1].base.radiosity.detail_level + 1);
            if ( permutation == 1 )
                permutation = (shader[1].base.radiosity.flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;

        default:
            /* chicago_extended shares the chicago flags layout */
            permutation = (int16_t)(shader[1].base.radiosity.detail_level + 1);
            if ( permutation == 1 )
                permutation = (shader[1].base.radiosity.flags & (1u << _shader_transparent_chicago_first_map_is_in_screenspace_bit)) != 0;
            if ( (shader->base.radiosity.flags & (1u << _shader_radiosity_FILTHY_transparent_lit_bit)) != 0 )
                return 5;
            break;
    }
    return permutation;
}
