/* shader_transparent_chicago_extended_create @0x83830F28 — byte-for-byte structural mirror of
 * shader_transparent_chicago_create (see that file for the full derivation of the by-value shader_stage_cfg
 * packing, the selector->op CTR jump table, and the is_cube carry idiom). It translates a
 * shader_transparent_chicago_extended tag the same way — building up to three shader_stage_cfg entries from
 * the tag's texture-stage array, deriving the additional-op opcode/argument from the tag's first-map fields,
 * and marking a cube shader when detail level is non-zero — and binds the result via set_chicago_shader.
 * Returns 1 when a config was produced, 0 when the tag has no stages.
 *
 * DEVIATION: identical to shader_transparent_chicago_create — Hex-Rays modeled the three 6-byte by-value
 * args as one __int128 shift-register (they are three consecutive stride-6 shader_stage_cfg structs), the
 * selector>7 default reads an uninitialized stack slot (genuine shipped bug, reproduced), and is_cube uses
 * the addic/subfe carry idiom = (detail_level != 0). The tag fields map onto the DB
 * shader_transparent_chicago_extended layout: chicago_extended.type (u16) = "detail_level",
 * chicago_extended.framebuffer_blend_function (u16) = additional-op selector,
 * chicago_extended.framebuffer_fade_mode (i16) = additional-op argument, chicago_extended.maps.count =
 * stage count, chicago_extended.maps.address = stage array (shader_transparent_chicago_map[], stride 220);
 * per-stage +0x00 flags (bit1 = a_replicate), +0x2C color_func, +0x2E alpha_func.
 *
 * DEVIATION: the stage-count load is a full 32-bit lwz truncated by extsh to a signed 16-bit value, i.e.
 * (__int16)maps.count — reproduced with the cast (a bare *(__int16*) would read the HIGH halfword on BE). */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_transparent_chicago_extended.h"
#include "headers/chicago_shaders.h"
#include "headers/shader_transparent_chicago_map.h"   /* shader_stage_cfg, set_chicago_shader */
#include "headers/shader_transparent_chicago_map_flags.h"
#include "headers/shader_framebuffer_blend_function.h"

int shader_transparent_chicago_extended_create(const shader *shader, int use_additional_op)
{
    const shader_transparent_chicago_extended *chicago =
        (const shader_transparent_chicago_extended *)shader;
    int stage_count = (int16_t)chicago->chicago_extended.maps.count;
    if ( stage_count <= 0 )
        return 0;

    shader_stage_cfg stages[3];
    stages[0].color_func = 0;
    stages[0].alpha_func = 0;
    stages[0].a_replicate = 0;
    stages[1] = stages[0];
    stages[2] = stages[0];

    if ( stage_count - 1 > 0 )
    {
        shader_transparent_chicago_map *stage_array =
            (shader_transparent_chicago_map *)chicago->chicago_extended.maps.address;
        for ( int i = 0; i < stage_count - 1; i++ )
        {
            shader_transparent_chicago_map *stage = &stage_array[i];
            stages[i].color_func = stage->color_function;
            stages[i].alpha_func = stage->alpha_function;
            stages[i].a_replicate = (stage->flags >> _shader_transparent_chicago_map_alpha_replicate_bit) & 1;
        }
    }

    int additional_op_arg;
    int additional_op;
    if ( !use_additional_op )
    {
        additional_op_arg = 3;
        additional_op = 0;
    }
    else
    {
        uint16_t selector = (uint16_t)chicago->chicago_extended.framebuffer_blend_function;
        additional_op_arg = chicago->chicago_extended.framebuffer_fade_mode;
        int uninitialized_op;   /* DEVIATION: shipped uninitialized-stack read for selector > 7 */
        switch ( selector )
        {
            case _shader_framebuffer_blend_function_alpha_blend:  additional_op = 0; break;
            case _shader_framebuffer_blend_function_multiply:  additional_op = 1; break;
            case _shader_framebuffer_blend_function_double_multiply:  additional_op = 2; break;
            case _shader_framebuffer_blend_function_add:  additional_op = 3; break;
            case _shader_framebuffer_blend_function_reverse_subtract:  additional_op = 3; break;
            case _shader_framebuffer_blend_function_min:  additional_op = 1; break;
            case _shader_framebuffer_blend_function_max:  additional_op = 3; break;
            case _shader_framebuffer_blend_function_alpha_multiply_add:  additional_op = 4; break;
            default: additional_op = uninitialized_op; break;
        }
    }

    int detail_level = (uint16_t)chicago->chicago_extended.type;
    set_chicago_shader(detail_level != 0, stages[0], stages[1], stages[2], additional_op_arg, additional_op);
    return 1;
}
