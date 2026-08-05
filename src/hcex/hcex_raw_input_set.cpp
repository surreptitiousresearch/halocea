/* hcex_raw_input_set @0x823C1640 — record one controller's raw pad state (from a
 * haloRAW_INPUT_X360) into the HCEX per-gamepad shadow arrays, unpacking the 16-bit button
 * bitmask into individual booleans in hcex_gamepad_buttons[gamepadId][0..15]. Counterpart to
 * hcex_raw_input_get. */

extern char  hcex_gamepad_buttons[4][16];
extern int16_t hcex_sThumbLX[4];
extern int16_t hcex_sThumbLY[4];
extern int16_t hcex_sThumbRX[4];
extern int16_t hcex_sThumbRY[4];

#include <stdint.h>
#include "../headers/hcex/haloRAW_INPUT_X360.h"

void hcex_raw_input_set(int gamepadId, const haloRAW_INPUT_X360 *inp)
{
    for ( int bit = 0; bit < 16; ++bit )
        hcex_gamepad_buttons[gamepadId][bit] = (inp->buttons & (1 << bit)) != 0;

    hcex_sThumbLX[gamepadId] = inp->sThumbLX;
    hcex_sThumbLY[gamepadId] = inp->sThumbLY;
    hcex_sThumbRX[gamepadId] = inp->sThumbRX;
    hcex_sThumbRY[gamepadId] = inp->sThumbRY;
}
