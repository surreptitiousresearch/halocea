/* scenario_material_definition_get @0x837034B0 — return the material definition for a global material type.
 * Valid indices select directly into the game globals' materials block (884-byte records). Out-of-range or
 * negative indices fall back to a shared default whose melee-hit sound is lazily cleared to "none" on first use.
 *
 * DEVIATION: the fallback path (0x837034E0-0x83703504) is byte-for-byte the same code as
 * default_material_definition_get (0x83703488) — not a call to it, the compiler just duplicated the body —
 * against the SAME `default_material_definition`/`initialized` symbols, so this file externs that shared
 * storage from default_material_definition_get.c instead of keeping a private static copy. */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/material_definition.h"
#include "headers/blam_data_globals.h"


extern material_definition default_material_definition;
extern uint8_t initialized; /* byte-wide in binary: lbz @0x837034E8, stb @0x83703500 */

material_definition *scenario_material_definition_get(int16_t material_type)
{
    if ( material_type >= 0 && material_type < global_game_globals->materials.count )
        /* recovered: (char *)materials.address + 884 * material_type -> [material_type] (884 == sizeof(material_definition)) */
        return &((material_definition *)global_game_globals->materials.address)[material_type];

    if ( !initialized )
    {
        default_material_definition.melee_hit_sound.index = -1;
        initialized = 1;
    }
    return &default_material_definition;
}
