// apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR>::apCOUNTER_TIME_CALL_WRP @0x82A3B5C0 and
// ::~apCOUNTER_TIME_CALL_WRP @0x82A3B67C — re-entrant variant: only records the start timestamp
// on the outermost entry (depth<=0) and only accumulates elapsed time once depth unwinds back to
// 0 on exit; every other (de)activation just adjusts the per-processor `depth` counter.
#include <stdint.h>
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

extern int osGetCurThreadProcessor(); /* DEVIATION: symbol is ?osGetCurThreadProcessor@@YAHXZ (C++-mangled, int) */
extern uint64_t osGetPerfCounter(); /* DEVIATION: was extern "C" unsigned int -- symbol ?osGetPerfCounter@@YA_KXZ (C++-mangled, uint64_t) */

template<>
apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR>::apCOUNTER_TIME_CALL_WRP(apCOUNTER_TIME_RECUR *cnt)
{
    counter = cnt;

    apCOUNTER_UNIT &unit0 = cnt->counterUnits[0];
    unsigned char   s0    = unit0.state.state;
    if (s0 & 0x40) s0 |= 0x02;
    if (s0 & 0x02) {
        unsigned int proc = osGetCurThreadProcessor();
        apCOUNTER_TIME_RECUR::tmDATA &slot = cnt->tmData[proc];
        if (slot.depth <= 0) {
            slot.start = osGetPerfCounter();
            slot.depth = 1;
        } else {
            ++slot.depth;
        }
    }
}

template<>
apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR>::~apCOUNTER_TIME_CALL_WRP()
{
    apCOUNTER_UNIT &unit0 = counter->counterUnits[0];
    unsigned char   s0    = unit0.state.state;
    if (s0 & 0x40) s0 |= 0x02;
    if (s0 & 0x02) {
        unsigned int proc = osGetCurThreadProcessor();
        apCOUNTER_TIME_RECUR::tmDATA &slot = counter->tmData[proc];
        if (slot.depth <= 1) {
            int64_t now = osGetPerfCounter();
            slot.sum += now - slot.start;
            --slot.depth;
        } else {
            --slot.depth;
        }
    }
}
