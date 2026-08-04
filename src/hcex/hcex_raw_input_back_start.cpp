/* hcex_raw_input_back_start @0x823C1600 — true if the given controller's button bitmask holds
 * Start (bit 12 = 0x1000) or Back (bit 13 = 0x2000) — the coop combo hcex_coop_process_raw_input
 * watches for to offer a cinematic skip. The bitmask is packed by gamepad_button index (see
 * hcex_raw_input_get): bit 12/13 = start/back per DB enum $F8C0FA1066F6E2019BAA777490D9D204.
 * DEVIATION: the decompiler typed the parameter `__int16`; the mangled name
 * (?hcex_raw_input_back_start@@YA_NK@Z) gives `bool (unsigned long)` — widened here to match. */

#include "../headers/gamepad_button.h"

int hcex_raw_input_back_start(unsigned int buttons)
{
    return (buttons & (1u << _gamepad_binary_button_start)) != 0
        || (buttons & (1u << _gamepad_binary_button_back)) != 0;
}
