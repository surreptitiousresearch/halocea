/* editor_camera_set_mode @0x837C3C28 — switch the editor camera's active translate mode. If the editor camera
 * exists and the mode is actually changing, runs the outgoing mode's "end" hook (translate_funcs[old][0]) and
 * the incoming mode's "begin" hook (translate_funcs[new][1]), each passed the editor camera. Always records
 * the new mode and prints its name to the console.
 *
 * DEVIATION: the decompiler rendered the translate_funcs calls as taking no arguments; disasm confirms
 * editor_camera (loaded into r3 immediately before each bctrl) is passed as the implicit camera argument,
 * matching the translate_funcs[][2] signature already established in editor_camera_new.c/editor_camera_update.c. */

#include <stdint.h>
#include "headers/flying_camera.h"
#include "headers/blam_data_globals.h"

extern void (*translate_funcs[][2])(flying_camera *camera);

extern void console_printf(uint8_t clear, const char *format, ...);

void editor_camera_set_mode(int16_t mode)
{
    if ( editor_camera && camera_mode != mode )
    {
        if ( camera_mode )
            translate_funcs[camera_mode][0](editor_camera);
        if ( mode )
            translate_funcs[mode][1](editor_camera);
    }

    camera_mode = mode;
    console_printf(0, mode_str[mode]);
}
