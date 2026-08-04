#include "headers/camera_script_globals.h"

int scripted_camera_next_camera_point(void)
{
    return (unsigned short)camera_script_globals.camera_point_index;
}
