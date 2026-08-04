// aiTASK_DATA_WAITTIME::DbgPrintInfo @ 0x83261100
// ?DbgPrintInfo@aiTASK_DATA_WAITTIME@@MAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITTIME.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // ds — format into a fresh dsTSTRING  boundary

// Protected virtual: append the remaining wait time (one decimal) to `out`.
void aiTASK_DATA_WAITTIME::DbgPrintInfo(dsTSTRING<char> *out)
{
    dsTSTRING<char> text = dsSPrintf("%.1f", timer);
    *out += text;
}
