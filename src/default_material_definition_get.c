/* default_material_definition_get @0x83703488 — returns the shared fallback material_definition used when a
 * material index is out of range, lazily clearing its melee_hit_sound to "none" on first use. Owns the shared
 * `default_material_definition`/`initialized` storage — disasm confirms scenario_material_definition_get.c's
 * fallback path (0x837034E0-0x83703504) duplicates this exact same code (not a call) against the SAME symbols,
 * so both translation units extern the storage from here rather than each keeping a private copy. */

#include <stdint.h>
#include "headers/material_definition.h"

material_definition default_material_definition;
uint8_t initialized; /* byte-wide in binary: lbz @0x83703490 / stb @0x837034A8 (and lbz/stb in the sibling) */

material_definition * default_material_definition_get(void)
{
    if (!initialized)
    {
        default_material_definition.melee_hit_sound.index = -1;
        initialized = 1;
    }
    return &default_material_definition;
}
