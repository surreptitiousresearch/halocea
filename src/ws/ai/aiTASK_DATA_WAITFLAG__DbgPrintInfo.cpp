// aiTASK_DATA_WAITFLAG::DbgPrintInfo @ 0x832611D8
// ?DbgPrintInfo@aiTASK_DATA_WAITFLAG@@MAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITFLAG.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // ds — format into a fresh dsTSTRING  boundary

// Protected virtual: append the awaited flag name in quotes to `out`.
void aiTASK_DATA_WAITFLAG::DbgPrintInfo(dsTSTRING<char> *out)
{
    dsTSTRING<char> text = dsSPrintf("'%s'", flag.CStr());
    *out += text;
}
