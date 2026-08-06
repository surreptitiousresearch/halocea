/* editor_camera_bump_speed @0x837C3A88 — cycles the editor debug camera through a 5-entry speed-multiplier
 * table and prints the new speed.
 *
 * DEVIATION: the decompiler mislabeled the `multiple` global's high-part load as
 * `&debug_obstacle_path_start_point_y_definition` (a coincidental symbol collision at the referenced
 * .data address). disasm_range(0x837C3AA0,0x837C3B10) confirms the real globals are the DB's own
 * auto-named `index`/`multiple`/`speed`, and the message is "speed is now x%f". */

#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

/* DEVIATION: the local extern typed `multiple` as float[5]; the elements are signed 32-bit INTEGERS
 * (disasm 0x837C3AE0-0x837C3AF4: `lwzx` then `extsw`/`fcfid`/`frsp` — an explicit int-to-float
 * conversion a float array would not need). Declared canonically by headers/blam_data_globals.h
 * (const int32_t[5], def src/data/multiple.c); `speed = multiple[index]` now performs that
 * conversion implicitly. */

extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void editor_camera_bump_speed(void)
{
    index = (index + 1) % 5;
    speed = multiple[index];
    terminal_printf(global_real_argb_white, "speed is now x%f", speed);
}
