#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h" // _apLog
#include "../../headers/ws/ai/AI_smt_boundary.h"    // AI::GetSmtObjFromActor
#include "../../headers/ws/ia/iaIACTOR.h"           // iaIACTOR::GetName
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::implRegisterDeathSmtObject(iaIACTOR*) @ 0x8325F590
// ?implRegisterDeathSmtObject@aiPLANNER@@UAAXPAViaIACTOR@@@Z (virtual)
//
// Worker for the "RegisterDeathSmtObject" SSL function: resolve `actor`'s smart object and, unless it
// is already present, append it to the animated-death smart-object list. Logs the resolver's error
// string on failure, or a "already registered" warning (naming the actor) on a duplicate.
void aiPLANNER::implRegisterDeathSmtObject(iaIACTOR *actor)
{
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj;
    dsTSTRING<char> errHint; // empty

    if (AI::GetSmtObjFromActor(actor, smtObj, errHint))
    {
        int nElem = animDeathSmtObjects.nElem;
        bool alreadyRegistered = false;
        for (int i = 0; i < nElem; ++i)
        {
            if (animDeathSmtObjects[i].pointee == smtObj.pointee)
            {
                alreadyRegistered = true;
                break;
            }
        }

        if (!alreadyRegistered)
            animDeathSmtObjects.PushBack(smtObj);
        else
            _apLog("~AI,Ssl,Error~$AI.RegisterDeathSmtObject(): smart object '%s' already registered!",
                   actor->GetName().pBuffer->str);
    }
    else
    {
        _apLog("~AI,Ssl,Error~$AI.RegisterDeathSmtObject(): %s", errHint.pBuffer->str);
    }
    // smtObj / errHint release their referents on scope exit (dsSHARED_PTR::Reset / ~dsTSTRING).
}
