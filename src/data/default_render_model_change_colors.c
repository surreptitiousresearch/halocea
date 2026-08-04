/* default_render_model_change_colors @ 0x844A5928 — default (black) change-color block used by
 * render_model when the caller passes no change_colors (the r22 NULL default at 0x83799328 =
 * default_function_values+0x58). data-bss zero-fill, DB type real_rgb_color (12 bytes).
 * Formerly misnamed default_render_model_function_colors — the render_model reconstruction had
 * shifted the parameter names by one from arg4 on (deferred-drain B6). */
#include "../headers/real_rgb_color.h"

real_rgb_color default_render_model_change_colors;
