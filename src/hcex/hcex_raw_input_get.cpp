/* hcex_raw_input_get @0x823C1750 — read one controller's raw pad state back out of the HCEX
 * per-gamepad shadow arrays into a haloRAW_INPUT_X360, repacking the 16 button booleans into a
 * bitmask. Counterpart to hcex_raw_input_set. */

extern char  hcex_gamepad_buttons[4][16];
extern __int16 hcex_sThumbLX[4];
extern __int16 hcex_sThumbLY[4];
extern __int16 hcex_sThumbRX[4];
extern __int16 hcex_sThumbRY[4];

#include "../headers/hcex/haloRAW_INPUT_X360.h"

void hcex_raw_input_get(int gamepadId, haloRAW_INPUT_X360 *inp)
{
    inp->buttons = 0;
    for ( int bit = 0; bit < 16; ++bit )
    {
        if ( hcex_gamepad_buttons[gamepadId][bit] )
            inp->buttons |= 1 << bit;
    }

    inp->sThumbLX = hcex_sThumbLX[gamepadId];
    inp->sThumbLY = hcex_sThumbLY[gamepadId];
    inp->sThumbRX = hcex_sThumbRX[gamepadId];
    inp->sThumbRY = hcex_sThumbRY[gamepadId];
}
