#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiGOAL_BASE::BuildCompoundPath @ 0x831792D8
// ?BuildCompoundPath@aiGOAL_BASE@@UAA_NABV?$dsTSTRING@D@@PAVnavPATH@@_NAAV2@@Z
//
// Base-class default: this goal module cannot build paths — report it disabled by writing
// "disabled" into `errHint` and returning false. The disassembly's inlined refcount juggling is
// just the dsTSTRING copy-assignment of a temporary "disabled" into errHint.
bool aiGOAL_BASE::BuildCompoundPath(const dsTSTRING<char> &dest, navPATH *pPath,
                                   bool isNeedHO, dsTSTRING<char> &errHint)
{
    (void)dest;
    (void)pPath;
    (void)isNeedHO;
    errHint = dsTSTRING<char>("disabled");
    return false;
}
