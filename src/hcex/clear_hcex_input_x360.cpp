#include <string.h>
#include "../headers/blam_data_globals.h"

/* clear_hcex_input_x360 @ 0x823C1540 — zero the digested X360 gamepad state HCEX feeds into the
 * legacy Blam input path: the 4x16 button bitmask and all four per-controller thumbstick-axis
 * arrays. (The decompiler types the return as __int16* — an artifact of the tail store leaving the
 * last written address in r3; the function is really void, matching every call site and the extern
 * declaration in haloENGINE_CONTROL_boundary.h / hcex_update_gamepad_x360.c.) */
void clear_hcex_input_x360(void)
{
    memset(hcex_gamepad_buttons, 0, sizeof(hcex_gamepad_buttons));
    memset(hcex_sThumbLX, 0, sizeof(hcex_sThumbLX));
    memset(hcex_sThumbLY, 0, sizeof(hcex_sThumbLY));
    memset(hcex_sThumbRX, 0, sizeof(hcex_sThumbRX));
    memset(hcex_sThumbRY, 0, sizeof(hcex_sThumbRY));
}
