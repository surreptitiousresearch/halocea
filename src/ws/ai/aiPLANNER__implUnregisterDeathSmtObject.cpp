#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h" // _apLog
#include "../../headers/ws/ai/AI_smt_boundary.h"    // AI::GetSmtObjFromActor
#include "../../headers/ws/ia/iaIACTOR.h"           // iaIACTOR::GetName
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::implUnregisterDeathSmtObject(iaIACTOR*) @ 0x8325F348
// ?implUnregisterDeathSmtObject@aiPLANNER@@UAAXPAViaIACTOR@@@Z (virtual)
//
// Worker for the "UnregisterDeathSmtObject" SSL function: resolve `actor`'s smart object and remove it
// from the animated-death smart-object list. Logs the resolver's error string on failure, or a "not
// registered" warning (naming the actor) when the object is absent from the list.
void aiPLANNER::implUnregisterDeathSmtObject(iaIACTOR *actor)
{
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj;
    dsTSTRING<char> errHint; // empty

    if (AI::GetSmtObjFromActor(actor, smtObj, errHint))
    {
        int nElem = animDeathSmtObjects.nElem;
        int foundIdx = -1;
        for (int i = 0; i < nElem; ++i)
        {
            if (animDeathSmtObjects[i].pointee == smtObj.pointee)
            {
                foundIdx = i;
                break;
            }
        }

        if (foundIdx >= 0)
            animDeathSmtObjects.Erase(foundIdx, 1);
        else
            _apLog("~AI,Ssl,Error~$AI.UnregisterDeathSmtObject(): smart object '%s' not registered!",
                   actor->GetName().pBuffer->str);
    }
    else
    {
        _apLog("~AI,Ssl,Error~$AI.UnregisterDeathSmtObject(): %s", errHint.pBuffer->str);
    }
    // smtObj / errHint release their referents on scope exit (dsSHARED_PTR::Reset / ~dsTSTRING).
}
