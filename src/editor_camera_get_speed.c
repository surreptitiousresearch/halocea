#include "headers/blam_data_globals.h"
/* DEVIATION: disasm (837C3B18) shows lfs/fctiwz/stfd/lwz idiom for (int)(float)speed;
 * 'speed' is the symbolic name IDA assigned to the local float global. */

int editor_camera_get_speed(void)
{
    return (int)speed;
}
