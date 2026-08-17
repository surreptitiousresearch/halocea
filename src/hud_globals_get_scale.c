/* hud_globals_get_scale @0x8379E100 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
/* hud_globals_get_scale 0x8379E100 — return the global HUD scale factor. The in_multiplayer parameter is
 * vestigial: the body unconditionally loads the single float hcex_hud_globals_scale and returns it.
 *
 * DEVIATION: the decompiler modeled the lone `lfs` as a double load and returned its high word
 * (`*((float*)&v1 + 1)`); the disassembly is a single-precision load of hcex_hud_globals_scale. */


float hud_globals_get_scale(uint8_t in_multiplayer)
{
    return hcex_hud_globals_scale;
}
