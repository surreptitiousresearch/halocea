#include "../../headers/ws/ai/aiTRASH_BIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/gs/gsLVL_SYSTEM_boundary.h" // scnSCENE, gsScenePtr, _apLog
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiTRASH_BIN::Update @ 0x832AB008
// ?Update@aiTRASH_BIN@@QAAXXZ
//
// Flush the deferred-destruction queues. If the brain is flagged for removal (AI_DIEST 0x200) fire
// the "REMOVE" SSL OnComplete and tear it down instantly; otherwise, if it has drifted outside the
// scene box, log and kill it with reason "REMOVE". Then destroy every queued behaviour and enemy
// module (vtable slot-0 scalar-deleting destructor) and empty both queues.
void aiTRASH_BIN::Update()
{
    aiBRAIN *brain = pBrain;
    if ((brain->st.pointee->die.val >> 9) & 1) // AI_DIEST: pending removal
    {
        dsTSTRING<char> reason("REMOVE");
        pBrain->mind.CallSSL_OnComplete(reason);
        pBrain->TermInstant();
    }
    else
    {
        m3dV pos = brain->GetPos();
        if (!gsScenePtr->IsBelongPosSceneBox(&pos))
        {
            dsTSTRING<char> name;
            pBrain->GetName(&name);
            _apLog("~AI~ %s was removed from the scene (does not belong to the scene box)", name.CStr());
            dsTSTRING<char> reason("REMOVE");
            pBrain->Die(reason);
        }
    }

    int nBhv = arrBhv.nElem;
    for (int i = 0; i < nBhv; ++i)
    {
        bhvBEHAV *b = arrBhv[i];
        if (b)
            (*reinterpret_cast<void (**)(bhvBEHAV *, int)>(*reinterpret_cast<void **>(b)))(b, 1);
    }
    arrBhv.nElem = 0;

    int nEne = arrEne.nElem;
    for (int k = 0; k < nEne; ++k)
    {
        aiENEMY_BASE *e = arrEne[k];
        if (e)
            (*reinterpret_cast<void (**)(aiENEMY_BASE *, int)>(*reinterpret_cast<void **>(e)))(e, 1);
    }
    arrEne.nElem = 0;
}
