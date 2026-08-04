/* hs_runtime_dispose_from_old_map @0x8368CB50 — tear down per-map HaloScript runtime state: invalidate the
 * thread array and delete every non-external (map-defined) global datum, then mark the runtime uninitialized.
 *
 * Deviation: database types this as returning data_array*, but the value is ABI residue; written as void. */

#include "headers/data_array.h"

extern __int16 hs_external_global_count;
#include "headers/hs_runtime_globals.h"
#include "headers/blam_data_globals.h"
extern void data_make_invalid(data_array *data);
extern void *datum_try_and_get(const data_array *data, int index);
extern void datum_delete(data_array *data, int index);

void hs_runtime_dispose_from_old_map(void)
{
    data_make_invalid(hs_thread_data);
    for ( int i = hs_external_global_count; i < hs_global_data->count; i = (__int16)(i + 1) )
    {
        if ( datum_try_and_get(hs_global_data, i) )
            datum_delete(hs_global_data, i);
    }
    hs_runtime_globals.initialized = 0;
}
