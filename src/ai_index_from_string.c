/* ai_index_from_string @0x8376ECE0 — resolve an AI designator string to a packed AI index.
 *   "none"                       -> -1 (success)
 *   "<encounter>"                -> encounter index in the low 16 bits
 *   "<encounter>/<squad>"        -> encounter | (squad << 16) with the 0x80000000 (squad) flag
 *   "<encounter>/<platoon>"      -> encounter | (platoon << 16) with the 0x40000000 (platoon) flag
 * Returns TRUE when a value (including the "none" sentinel) was resolved. The packed index is written to
 * *ai_index_reference. */

#include <stdint.h>
#include <string.h>
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/ai_index_scope.h"

typedef struct encounter_definition encounter_definition;

/* strrchr declared by <string.h> */
/* strncpy declared by <string.h> */
extern int scenario_get_encounter_by_name(scenario *scenario, const char *encounter_name);
extern int encounter_definition_get_squad_by_name(encounter_definition *encounter, const char *squad_name);
extern int encounter_definition_get_platoon_by_name(encounter_definition *encounter, const char *platoon_name);

uint8_t ai_index_from_string(scenario *scenario, const char *ai_string, unsigned int *ai_index_reference)
{
    unsigned int packed = (unsigned int)-1;

    if ( !stricmp(ai_string, "none") )
    {
        *ai_index_reference = (unsigned int)-1;
        return 1;
    }

    char *slash = strrchr(ai_string, '/');
    if ( !slash )
    {
        int encounter = scenario_get_encounter_by_name(scenario, ai_string);
        if ( encounter != -1 )
            packed = (uint16_t)encounter;
    }
    else
    {
        int prefix_length = slash - ai_string;
        if ( prefix_length <= 31 )
        {
            char encounter_name[96];
            strncpy(encounter_name, ai_string, prefix_length);
            encounter_name[prefix_length] = 0;

            int encounter = scenario_get_encounter_by_name(scenario, encounter_name);
            if ( encounter != -1 )
            {
                uint16_t encounter_index = encounter;
                encounter_definition *definition =
                    &((encounter_definition *)scenario->ai_encounters.address)[encounter];

                int sub = encounter_definition_get_squad_by_name(definition, slash + 1);
                unsigned int type_flag;
                if ( sub != -1 )
                {
                    type_flag = (unsigned int)_ai_index_squad << 30;    /* 0x80000000 */
                }
                else
                {
                    sub = encounter_definition_get_platoon_by_name(definition, slash + 1);
                    type_flag = (unsigned int)_ai_index_platoon << 30;  /* 0x40000000 */
                }
                if ( sub != -1 )
                    packed = ((sub << 16) & 0xFF0000) | (type_flag & 0xFF00FFFF) | encounter_index;
            }
        }
    }

    *ai_index_reference = packed;
    return packed != (unsigned int)-1;
}
