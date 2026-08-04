#include "scnSCENE.h"
#include "../ds/ds_assert_boundary.h"

extern "C" int stricmp(const char *a, const char *b); // boundary — case-insensitive CRT compare

// ?FindInstName@scnSCENE@@QAAPAVanimINST@@PBD@Z  (0x8253CFA8) — case-insensitive scan of the live
// list for the first instance whose name matches `name`. A null per-instance name is treated as
// the shared empty string (dsStrongAssertMessage aliases the same 0x8200155A empty-string byte
// used by the STRONG_ASSERT machinery). An empty/null `name` never matches.
animINST *scnSCENE::FindInstName(const char *name)
{
    if (!name || !*name)
        return nullptr;

    animINST *inst = nullptr;
    for (;;)
    {
        inst = inst ? inst->next : this->pFirstInst;
        if (!inst)
            break;

        const char *instName = inst->name;
        if (!instName)
            instName = dsStrongAssertMessage;
        if (!stricmp(instName, name))
            return inst;
    }
    return nullptr;
}
