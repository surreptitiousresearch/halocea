/* shader_transparent_chicago_create @0x83830D70 — translate a shader_transparent_chicago tag into the
 * fixed-function-emulation pixel-shader state and bind it via set_chicago_shader. It builds up to three
 * shader_stage_cfg entries (color/alpha combine funcs + alpha-replicate bit) from the tag's texture-stage
 * ("maps") array, derives the "additional operation" opcode/argument from the tag's first-map fields, and
 * marks the shader as a cube shader when its detail level is non-zero. Returns 1 when a config was produced,
 * 0 when the tag has no stages.
 *
 * DEVIATION: this is the deferred "__int128 by-value packing" case. Hex-Rays modeled the three 6-byte
 * shader_stage_cfg by-value arguments to set_chicago_shader as a single __int128/__int64 shift-register
 * (HIDWORD/DWORD1/DWORD2 with <<16 packing) because 6-byte structs are passed in 64-bit GPRs. Disasm at
 * 0x83830DC8 (the fill loop) and 0x83830EC0 (the pack) shows the packing is simply three consecutive
 * shader_stage_cfg structs written at stride 6, so the real call passes stages[0..2] by value —
 * reconstructed as such. The additional-op selector at shader+0x2C drives a CTR jump table (disasm
 * 0x83830E34) mapping selector 0..7 -> op {0,1,2,3,3,1,3,4}; selector>7 reads an UNINITIALIZED stack slot
 * (v20/var_60, never written) — a genuine shipped uninit read, reproduced faithfully. is_cube uses the
 * addic/subfe carry idiom = (detail_level != 0). The tag fields map onto the DB shader_transparent_chicago
 * layout (the decompiler's shader[1]/shader[2] array indexing was a mis-typing artifact): chicago.type
 * (u16) = "detail_level", chicago.framebuffer_blend_function (u16) = additional-op selector,
 * chicago.framebuffer_fade_mode (i16) = additional-op argument, chicago.maps.count = stage count,
 * chicago.maps.address = stage array (shader_transparent_chicago_map[], stride 220); per-stage +0x00 flags
 * (bit1 = a_replicate), +0x2C color_func, +0x2E alpha_func.
 *
 * DEVIATION: the stage-count load is `lwz r11,0x54(r3)` then `extsh` — a full 32-bit load truncated to a
 * signed 16-bit value, i.e. (__int16)maps.count. The decompiler's `*(__int16*)(shader+0x54)` would read the
 * HIGH halfword on big-endian PPC; the (__int16) cast on maps.count reproduces the real low-halfword value. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_transparent_chicago.h"
#include "headers/chicago_shaders.h"
#include "headers/shader_transparent_chicago_map.h"   /* shader_stage_cfg, set_chicago_shader */
#include "headers/shader_transparent_chicago_map_flags.h"
#include "headers/shader_framebuffer_blend_function.h"

int shader_transparent_chicago_create(const shader *shader, int use_additional_op)
{
    const shader_transparent_chicago *chicago = (const shader_transparent_chicago *)shader;
    int stage_count = (int16_t)chicago->chicago.maps.count;
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
            (shader_transparent_chicago_map *)chicago->chicago.maps.address;
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
        uint16_t selector = (uint16_t)chicago->chicago.framebuffer_blend_function;
        additional_op_arg = chicago->chicago.framebuffer_fade_mode;
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

    int detail_level = (uint16_t)chicago->chicago.type;
    set_chicago_shader(detail_level != 0, stages[0], stages[1], stages[2], additional_op_arg, additional_op);
    return 1;
}
