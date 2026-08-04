// aiTASK_DATA_WAITBHV::DbgPrintInfo @ 0x83261018
// ?DbgPrintInfo@aiTASK_DATA_WAITBHV@@MAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_WAITBHV.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // ds — format into a fresh dsTSTRING  boundary

// Protected virtual: append the awaited behaviour name in quotes to `out`.
void aiTASK_DATA_WAITBHV::DbgPrintInfo(dsTSTRING<char> *out)
{
    dsTSTRING<char> text = dsSPrintf("'%s'", waitBhvName.CStr());
    *out += text;
}
