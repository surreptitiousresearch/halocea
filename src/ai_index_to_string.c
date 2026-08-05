/* ai_index_to_string @0x8376EE14 — format a packed AI index as a human-readable designator string: "none" for
 * -1, "<encounter>" for a plain encounter reference, "<encounter>/<platoon>" or "<encounter>/<squad>" for the
 * scoped forms (see ai_index_from_string.c for the packing scheme: bits 30-31 select scope, byte 1 is the
 * sub-index), or "<error>" for the reserved/invalid scope. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/platoon_definition.h"
#include "headers/squad_definition.h"
#include "headers/ai_index_scope.h"

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
    unsigned char sub_index = (unsigned char)(ai_index >> 8);

    switch ( ai_index >> 30 )
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
