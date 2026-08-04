/* editor_camera_bump_speed @0x837C3ACC — cycles the editor debug camera through a 5-entry speed-multiplier
 * table and prints the new speed.
 *
 * DEVIATION: the decompiler mislabeled the `multiple` global's high-part load as
 * `&debug_obstacle_path_start_point_y_definition` (a coincidental symbol collision at the referenced
 * .data address). disasm_range(0x837C3AA0,0x837C3B10) confirms the real globals are the DB's own
 * auto-named `index`/`multiple`/`speed`, and the message is "speed is now x%f". */

#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern float multiple[5];

extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void editor_camera_bump_speed(void)
{
    index = (index + 1) % 5;
    speed = multiple[index];
    terminal_printf(global_real_argb_white, "speed is now x%f", speed);
}
