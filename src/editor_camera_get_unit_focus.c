#include "headers/blam_data_globals.h"
/* editor_camera_get_unit_focus @0x837C3C18 — getter for the editor camera's focus unit index (see
 * editor_camera_set_unit_focus.c for the same global). */


int editor_camera_get_unit_focus(void)
{
    return unit_focus;
}
