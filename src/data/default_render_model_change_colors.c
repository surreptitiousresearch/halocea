/* default_render_model_change_colors @ 0x844A5928 (.data, 48 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 * default (black) change-color block used by
 * render_model when the caller passes no change_colors (the r22 NULL default at 0x83799328 =
 * default_function_values+0x58). data-bss zero-fill, DB type real_rgb_color (12 bytes).
 * Formerly misnamed default_render_model_function_colors — the render_model reconstruction had
 * shifted the parameter names by one from arg4 on (deferred-drain B6).
 */
#include "../headers/real_rgb_color.h"

real_rgb_color default_render_model_change_colors;
