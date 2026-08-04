// aiBASS::FindExclamation @ 0x83243970
// ?FindExclamation@aiBASS@@MAAABVexDATA@1@VdsSTRID@@@Z
#include "../../headers/ws/ai/aiBASS.h"

// Look up the configured exclamation category named `id`. When none matches, return a shared
// process-wide "invalid" default (negative priorities/cooldowns, invalid overlap) so callers can
// test the result field-wise without a null check.
const aiBASS::exDATA &aiBASS::FindExclamation(dsSTRID id)
{
    int nElem = exclamations.nElem;
    for (int i = 0; i < nElem; ++i)
        if (!(exclamations[i].name != id))
            return exclamations[i];

    static exDATA dummy;
    static bool dummyInit = false;
    if (!dummyInit)
    {
        dummyInit = true;
        dummy.name = dsSTRID();
        dummy.exMaxCount = -1;
        dummy.isAcceptForDead = false;
        dummy.overlap = exOVERLAP_INVALID;
        dummy.priority = -1.0f;
        dummy.aiCoolDown = -1.0f;
        dummy.exCoolDown = -1.0f;
    }
    return dummy;
}
