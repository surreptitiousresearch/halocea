#ifndef RASTERIZER_DRAWING_MODE_H
#define RASTERIZER_DRAWING_MODE_H

/* rasterizer_debug_options.drawing_mode values (debug BSP-visualization modes).
 * DB anonymous enum _24FBD9041AD144AEFA7D96007C187117 (verbatim). */
enum rasterizer_drawing_mode
{
    _rasterizer_drawing_mode_normal = 0,
    _rasterizer_drawing_mode_overdraw = 1,
    _rasterizer_drawing_mode_bump_color = 2,
    _rasterizer_drawing_mode_specular_mask = 3,
    _rasterizer_drawing_mode_specular_mask_times_bump_color = 4,
    _rasterizer_drawing_mode_diffuse_texture_times_bump_color = 5,
    _rasterizer_drawing_mode_bump_edge = 6,
    _rasterizer_drawing_mode_specular_mask_times_bump_edge = 7,
    _rasterizer_drawing_mode_diffuse_texture_times_bump_edge = 8,
    _rasterizer_drawing_mode_vectors = 9,
    NUMBER_OF_RASTERIZER_DRAWING_MODES = 10
};

#endif /* RASTERIZER_DRAWING_MODE_H */
