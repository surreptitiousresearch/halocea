#pragma once

/* rasterizer_set_stencil_mode argument enumeration.
 * DB anonymous enum _8A452671B8D38A830DFC6094F52659C7 (types_enum_values). */
enum rasterizer_stencil_mode
{
    _rasterizer_stencil_mode_none                       = 0,
    _rasterizer_stencil_mode_write                      = 1,
    _rasterizer_stencil_mode_reject                     = 2,
    _rasterizer_stencil_mode_reject_invert              = 3,
    _rasterizer_stencil_mode_write_alpha_tested_decal   = 4,
    _rasterizer_stencil_mode_reject_alpha_tested_decal  = 5,
    NUMBER_OF_RASTERIZR_STENCIL_MODES                   = 6
};
