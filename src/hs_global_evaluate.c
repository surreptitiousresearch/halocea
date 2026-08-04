/* hs_global_evaluate @0x8368E478 — reconcile (refresh) a HaloScript global from its backing engine state, then
 * return its current 4-byte value from hs_global_data. External globals (designator < 0, i.e.
 * HS_GLOBAL_EXTERNAL_FLAG set — the compiler tests the sign of the __int16 instead of the bit) index directly;
 * scenario globals are offset past the external globals. Each global value occupies 8 bytes; the value is
 * at +4. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_global_runtime.h"
#include "headers/blam_data_globals.h"

extern void hs_global_reconcile_read(int16_t global_designator);
extern __int16 hs_external_global_count;

int hs_global_evaluate(int16_t global_designator)
{
    hs_global_reconcile_read(global_designator);
    int index;
    if ( global_designator < 0 )
        index = HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator);
    else
        index = HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator) + hs_external_global_count;
    return DATA_ARRAY_ELEMENT(hs_global_data, hs_global_runtime, index)->value;
}
