// apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL>::apCOUNTER_TIME_CALL_WRP @0x823DF79C and
// ::~apCOUNTER_TIME_CALL_WRP @0x823DF974 — RAII start/stop of a call-time counter's two display
// units (unit 0 = "calls", unit 1 = "time/calls"). Unit state bit 0x40 ("late-added") is treated
// as also carrying bit 0x02 ("enabled") for the remainder of the scope. On start: unit0 enabled ->
// record tmData[curProcessor].start; unit1 enabled -> bump callsTotal. On either unit having a
// bound apPROFILER, poke Start()/Stop(). On destruction, unit0 accumulates elapsed time into
// tmData[curProcessor].sum.
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"
#include "../../headers/ws/os/osLOCK.h" // osGetCurThreadProcessor/osGetPerfCounter decls (os globals)

extern "C" unsigned int osGetCurThreadProcessor();
extern "C" unsigned int osGetPerfCounter();

template<>
apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL>::apCOUNTER_TIME_CALL_WRP(apCOUNTER_TIME_CALL *cnt)
{
    counter = cnt;

    apCOUNTER_UNIT &unit0 = cnt->counterUnits[0];
    unsigned char   s0    = unit0.state.state;
    if (s0 & 0x40) s0 |= 0x02;
    if (s0 & 0x02) {
        unsigned int proc = osGetCurThreadProcessor();
        cnt->tmData[proc].start = osGetPerfCounter();
    }

    apCOUNTER_UNIT &unit1 = cnt->counterUnits[1];
    unsigned char   s1    = unit1.state.state;
    if (s1 & 0x40) s1 |= 0x02;
    if (s1 & 0x02)
        ++cnt->callsTotal;

    if (unit0.state.state & 0x10 || unit0.myProfiler)
        if (unit0.myProfiler) unit0.myProfiler->Start();

    if (unit1.state.state & 0x10 || unit1.myProfiler)
        if (unit1.myProfiler) unit1.myProfiler->Start();
}

template<>
apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL>::~apCOUNTER_TIME_CALL_WRP()
{
    apCOUNTER_UNIT &unit0 = counter->counterUnits[0];
    unsigned char   s0    = unit0.state.state;
    if (s0 & 0x40) s0 |= 0x02;
    if (s0 & 0x02) {
        unsigned int proc = osGetCurThreadProcessor();
        unsigned int now  = osGetPerfCounter();
        counter->tmData[proc].sum += (__int64)now - counter->tmData[proc].start;
    }

    if (unit0.myProfiler) unit0.myProfiler->Stop();

    apCOUNTER_UNIT &unit1 = counter->counterUnits[1];
    if (unit1.myProfiler) unit1.myProfiler->Stop();
}
