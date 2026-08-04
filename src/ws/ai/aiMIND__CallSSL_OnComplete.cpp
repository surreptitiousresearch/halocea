#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiMIND::CallSSL_OnComplete @ 0x83229348
// ?CallSSL_OnComplete@aiMIND@@QAAXABV?$dsTSTRING@D@@@Z
//
// Fire the unit's SSL "OnComplete/OnDeath" script events exactly once. When the unit was actually
// killed by damage (die bit 0x10000), first fire the variant carrying the killing-damage name;
// then always fire the no-argument variant.
void aiMIND::CallSSL_OnComplete(const dsTSTRING<char> &nameDamage)
{
    aiBRAIN  *brain  = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *status = brain->st.pointee;

    // die bit 3 — OnComplete already fired: run only the first time.
    if (((static_cast<unsigned int>(status->die.val) >> 3) & 1) != 0)
        return;
    status->die.val |= 8u;

    const dscDESC *desc = brain->spDesc.pointee;

    // die bit 0x10000 — killed by damage: fire OnComplete with the damage name argument.
    if ((status->die.val & 0x10000) != 0)
    {
        dsDATA argData;
        argData.type = nullptr;
        // The decompiler boxes `nameDamage` through a dsDATA_SIMPLE_WRAPPER<dsTSTRING<char>> whose
        // operator dsDATA() is just this StoreValue<> — folded here to the equivalent direct call.
        argData.StoreValue<dsTSTRING<char> >(nameDamage);
        dsDATA *argv[1] = { &argData };
        // RAII: the returned sslERROR temporary and argData release themselves at end of statement /
        // scope, standing in for the decompiler's inlined refcount-decrement/dlFree/Destroy tails.
        brain->sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnComplete(desc), 1, argv);
    }

    brain->sslObject.HandleEvent(aiBRAIN_DESC_SslEvOnCompleteNoArg(desc), 0, nullptr);
}
