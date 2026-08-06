/* ai_index_to_string @0x8376EE00 — format a packed AI index as a human-readable designator string: "none" for
 * -1, "<encounter>" for a plain encounter reference, "<encounter>/<platoon>" or "<encounter>/<squad>" for the
 * scoped forms (see ai_index_from_string.c for the packing scheme: bits 30-31 select scope, byte 1 is the
 * sub-index), or "<error>" for the reserved/invalid scope.
 *
 * DEVIATION (entry point): the file header previously said @0x8376EE14, which lands mid-body (the `bne` of
 * the "none" test). funcs.address is 0x8376EE00.
 *
 * DEVIATION: the sub-index unpack was `(unsigned char)(ai_index >> 8)` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376EE60 (squad, stride 0xE8) and 0x8376EE7C (platoon, stride 0xAC)
 * are `extrwi rX, r11, 8, 8` (rlwinm SH=16 MB=24 ME=31, words 0x556A863E / 0x556B863E), i.e.
 * (x >> 16) & 0xFF. Now AI_INDEX_SUB_INDEX. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/platoon_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_index.h"

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern char *strncpy(char *dst, const char *src, unsigned int n);

void ai_index_to_string(unsigned int ai_index, scenario *scenario, char *buffer, unsigned int bufsize)
{
    if ( ai_index == (unsigned int)-1 )
    {
        strncpy(buffer, "none", bufsize);
        return;
    }

    encounter_definition *encounter =
        &((encounter_definition *)scenario->ai_encounters.address)[(uint16_t)ai_index];
    unsigned char sub_index = (unsigned char)AI_INDEX_SUB_INDEX(ai_index);

    switch ( AI_INDEX_SCOPE(ai_index) )
    {
    case _ai_index_encounter:
        _snprintf_0(buffer, bufsize, "%s", encounter->name);
        break;
    case _ai_index_platoon: /* explicit platoon (see ai_index_platoon_iterator_new.c) */
        _snprintf_0(buffer, bufsize, "%s/%s", encounter->name,
            ((const platoon_definition *)encounter->platoons.address)[sub_index].name);
        break;
    case 3: /* reserved/invalid scope (no DB name) */
        strncpy(buffer, "<error>", bufsize);
        break;
    default: /* _ai_index_squad — squad-scoped (see ai_index_squad_iterator_new.c) */
        _snprintf_0(buffer, bufsize, "%s/%s", encounter->name,
            ((const squad_definition *)encounter->squads.address)[sub_index].name);
        break;
    }
}
