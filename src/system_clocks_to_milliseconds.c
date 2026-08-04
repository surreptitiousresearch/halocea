/* system_clocks_to_milliseconds @ 0x83762CE8 — convert counter ticks to ms */

#include <stdint.h>
#include "headers/_LARGE_INTEGER.h"
#include "headers/blam_data_globals.h"

/* attested uint32_t(int64_t): mulli/divd are 64-bit signed ops on the r3 arg; the result is
 * explicitly truncated to 32 bits at the return (clrlwi r3, r8, 0 @83762CF8) */
uint32_t system_clocks_to_milliseconds(int64_t clocks)
{
    /* (clocks * 1000) / frequency — the decompiler rendered the *1000 scaling
     * obscurely via __SPAIR64__; the operation is a 64-bit tick->ms convert. */
    return (uint32_t)((clocks * 1000) / s_ClockFrequency.QuadPart);
}
