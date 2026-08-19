/* hs_inspect_enum @0x8368CA40 — format an enum HaloScript value into a text buffer. The enum index lives in the
 * high word of the 32-bit value word; the enum's identifier table is hs_enum_table[type-32]. Installed in
 * hs_type_inspectors[32..36] (game_difficulty/team/ai_default_state/actor_type/hud_corner). */

#include <stdint.h>
#include "headers/hs_enum_definition.h"
#include "headers/hs_type.h"

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_enum(int16_t type, int value, char *buffer)
{
    sprintf_0(buffer, "%s", hs_enum_table[type - first_hs_enum_type].identifiers[(short)(value >> 16)]);   /* SHIWORD(value) */
}
