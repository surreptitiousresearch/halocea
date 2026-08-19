/* hs_global_get_type @0x8372EEE8 — return the value type of a HaloScript global. The HS_GLOBAL_EXTERNAL_FLAG
 * bit selects an engine-defined external global (pointer table); otherwise it indexes the scenario's hs_globals
 * block (92-byte entries; type at word 16). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_internal.h"

int16_t hs_global_get_type(int16_t designator)
{
    if ( HS_GLOBAL_IS_EXTERNAL(designator) )
        return (uint16_t)hs_external_globals[HS_GLOBAL_DESIGNATOR_TO_INDEX(designator)]->type;
    return (uint16_t)((hs_global_internal *)global_scenario->hs_globals.address)[HS_GLOBAL_DESIGNATOR_TO_INDEX(designator)].type;
}
