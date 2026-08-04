#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiMIND::CallSSL_OnBattleChanged @ 0x832294A8
// ?CallSSL_OnBattleChanged@aiMIND@@QAAXXZ
//
// Fire the unit's SSL "OnBattleChanged" script event with the current in-combat state (mind bit
// 0x80). gen bit 0x2000000 is raised for the duration of the dispatch as a re-entrancy guard.
void aiMIND::CallSSL_OnBattleChanged()
{
    aiBRAIN  *brain  = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *status = brain->st.pointee;

    status->gen.val |= 0x2000000u; // enter OnBattleChanged dispatch

    bool inCombat = (status->mind.val & 0x80) != 0;

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<bool>(inCombat);
    dsDATA *argv[1] = { &argData };
    // RAII: the returned sslERROR temporary and argData release themselves at scope end (the
    // decompiler's inlined refcount-decrement/dlFree/Destroy tails).
    brain->sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnBattleChanged(brain->spDesc.pointee), 1, argv);

    status->gen.val &= ~0x2000000u; // leave OnBattleChanged dispatch
}
