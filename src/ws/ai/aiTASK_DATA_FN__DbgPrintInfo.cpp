// aiTASK_DATA_FN::DbgPrintInfo @ 0x83260F58
// ?DbgPrintInfo@aiTASK_DATA_FN@@MAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern dsTSTRING<char> dsSPrintf(const char *fmt, ...); // ds — format into a fresh dsTSTRING  boundary

// Protected virtual: append "$<objName>.<fnName>" (target object + scripted function) to `out`.
void aiTASK_DATA_FN::DbgPrintInfo(dsTSTRING<char> *out)
{
    dsTSTRING<char> text = dsSPrintf("$%s.%s", objName.CStr(), fnName.CStr());
    *out += text;
}
