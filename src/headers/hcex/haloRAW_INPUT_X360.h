#pragma once
#include <stdint.h>
/* haloRAW_INPUT_X360 — one frame's worth of raw Xbox 360 gamepad state as digested by the HCEX
 * bridge (hcex_update_gamepad_x360) and later replayed into the Blam input path
 * (hcex_coop_process_raw_input / hcex_raw_input_set / hcex_raw_input_get). DB-verified layout
 * (types_members haloRAW_INPUT_X360): buttons@0x00, sThumbLX@0x04, sThumbLY@0x06, sThumbRX@0x08,
 * sThumbRY@0x0A — 12 bytes. `buttons` is a 16-button bitmask (bit N == hcex_gamepad_buttons[*][N]);
 * only bits 0x1000 (back) and 0x2000 (start) are interpreted by name so far
 * (hcex_raw_input_back_start). */

typedef struct haloRAW_INPUT_X360
{
    unsigned int buttons;   /* 0x00 16-button bitmask */
    int16_t      sThumbLX;  /* 0x04 */
    int16_t      sThumbLY;  /* 0x06 */
    int16_t      sThumbRX;  /* 0x08 */
    int16_t      sThumbRY;  /* 0x0A */
} haloRAW_INPUT_X360;       /* 12 bytes */
