#pragma once

/* rasterizer_globals.current_lock_operation enumeration.
 * DB anonymous enum _7ACCC71F6D50F9CE3E65D961288CEFED (types_enum_values). */
enum rasterizer_lock_operation
{
    _rasterizer_lock_none            = 0,
    _rasterizer_lock_texture_changed = 1,
    _rasterizer_lock_vertexbuffer_new = 2,
    _rasterizer_lock_detail_objects  = 3,
    _rasterizer_lock_decal_update    = 4,
    _rasterizer_lock_decal_vertices  = 5,
    _rasterizer_lock_bink            = 6,
    _rasterizer_lock_ui              = 7,
    _rasterizer_lock_cinematics      = 8,
    _rasterizer_lock_koth            = 9,
    _rasterizer_lock_hud             = 10,
    _rasterizer_lock_flag            = 11,
    _rasterizer_lock_lightning       = 12,
    _rasterizer_lock_debug           = 13,
    _rasterizer_lock_text            = 14,
    _rasterizer_lock_contrail        = 15,
    _rasterizer_lock_sprite          = 16,
    _rasterizer_lock_bsp_switch      = 17,
    NUMBER_OF_RASTERIZER_LOCK_OPERATIONS = 18
};
