/* hs_global_get_name @0x8372EF30 — return the name of a HaloScript global. External globals
 * (HS_GLOBAL_EXTERNAL_FLAG bit) carry an explicit name pointer; scenario globals store the name as the first
 * field of their 92-byte hs_globals entry. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_internal.h"

const char *hs_global_get_name(int16_t designator)
{
    if ( HS_GLOBAL_IS_EXTERNAL(designator) )
        return hs_external_globals[HS_GLOBAL_DESIGNATOR_TO_INDEX(designator)]->name;
    return ((hs_global_internal *)global_scenario->hs_globals.address)[HS_GLOBAL_DESIGNATOR_TO_INDEX(designator)].name;
}
