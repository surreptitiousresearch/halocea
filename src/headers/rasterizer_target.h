#ifndef RASTERIZER_TARGET_H
#define RASTERIZER_TARGET_H

/* rasterizer render target enum — headers_ref _A4EFD520BC2FB551191CD2EF1763CD63 */
enum rasterizer_target
{
    _rasterizer_target_render_d3d            = 0,
    _rasterizer_target_render_primary        = 1,
    _rasterizer_target_render_secondary      = 2,
    _rasterizer_target_shadow_primary        = 3,
    _rasterizer_target_shadow_secondary      = 4,
    _rasterizer_target_motion_sensor_primary = 5,
    _rasterizer_target_sun_glow_primary      = 6,
    _rasterizer_target_sun_glow_secondary    = 7,
    _rasterizer_target_water                 = 8,
    _rasterizer_target_z                     = 9,
    NUMBER_OF_RASTERIZER_TARGETS             = 10,
};

#endif /* RASTERIZER_TARGET_H */

// rasterizer_target: DB-verified via types_enum_values _A4EFD520BC2FB551191CD2EF1763CD63 (anonymous compiled enum, names sans leading _)
