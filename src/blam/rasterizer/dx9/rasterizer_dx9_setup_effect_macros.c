/* rasterizer_dx9_setup_effect_macros @0x836A0CE0 — populate the global D3DX
 * effect macro table used when compiling .fx effects (xbox + pixel-shader
 * target profile), NULL-terminated. */

#include "headers/rasterizer_dx9_shader_table.h"

void rasterizer_dx9_setup_effect_macros(void)
{
    global_effect_macros[0].Name = "xbox";
    global_effect_macros[0].Definition = "1";
    global_effect_macros[1].Name = "PS_TGT";
    global_effect_macros[1].Definition = "ps_3_0";
    global_effect_macros[2].Name = 0;
    global_effect_macros[2].Definition = 0;
}
