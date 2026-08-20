#include <stdint.h>
#include "headers/blam_data_globals.h"
/* global_structure_bsp_index_get @0x83704048 — return the index of the currently-loaded structure BSP.
 * The load is `lhz` @0x8370404C (zero-extending) rather than lha, but that is not observable: every call
 * site re-normalizes r3 itself before comparing — `extsh r10,r3` @0x83758CB8, `extsh r10,r3` @0x83770850,
 * `extsh r8,r26` @0x836EA23C — or truncates it with a 16-bit store, which is MSVC's PPC convention for a
 * `short` return. int16_t is therefore the spelling the callers are compiled against; widening the return
 * to uint16_t would make those three comparisons 65535 != -1 and diverge from the binary. */


int16_t global_structure_bsp_index_get(void)
{
    return global_structure_bsp_index;
}
