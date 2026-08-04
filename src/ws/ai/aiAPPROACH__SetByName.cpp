#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiNAMES.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// _apLog — engine formatted log sink. boundary.
extern void _apLog(const char *fmt, ...);

// aiAPPROACH::SetByName(const dsTSTRING<char> &) @ 0x8329B120
// ?SetByName@aiAPPROACH@@UAA_NABV?$dsTSTRING@D@@@Z  (vtbl slot 0x08)
//
// Look up the named "approach" section in the owning brain's species descriptor and apply it.
// Returns false when the descriptor is missing, the section is absent, or ParsePS rejects it.
bool aiAPPROACH::SetByName(const dsTSTRING<char> &nameArg)
{
    dscDESC *desc = pBrain->spDesc.pointee;
    if (!desc)
        return false;

    // The descriptor embeds a psSECTION_INTERFACE (the parsed species config) at byte offset 1068
    // (dscDESC[89] over the descriptor's dscDESC[] view). Fetch the named sub-section from it.
    const psSECTION_INTERFACE *descSection =
        reinterpret_cast<const psSECTION_INTERFACE *>(
            reinterpret_cast<const unsigned char *>(desc) + 1068);
    psSECTION section = descSection->GetSection(nameArg.CStr(), 0);

    if (!section.pData)
        return false;

    // Faithful to the binary: ParsePS returning non-zero is the accept path (see ParsePS note).
    if (!ParsePS(section))
    {
        _apLog("~Error,AI,Ssl~%s: approach is rejected", pBrain->names.nameInst);
        return false;
    }

    state.val &= ~AI_APST_SOMEONE_IN_ZONE;
    name = nameArg;
    return true;
}
