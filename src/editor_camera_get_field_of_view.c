#include "headers/blam_data_globals.h"
extern float fov[];

/* DEVIATION: decompiler double-return idiom; disasm is lfsx (single-precision load) */
float editor_camera_get_field_of_view(void)
{
    return fov[camera_mode];
}
