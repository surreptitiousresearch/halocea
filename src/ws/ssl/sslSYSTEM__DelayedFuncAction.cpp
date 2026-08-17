/* ?DelayedFuncAction@sslSYSTEM@@QAAXW4DELACT@1@VdsSTRID@@VsslOBJ_REF@@@Z @0x825CE620 */
#include "sslSYSTEM.h"
#include "sslDELAYED_FUNCTION.h"
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// 0x825CE620 — sslSYSTEM::DelayedFuncAction. Apply `action` to every queued delayed call matching
// `obj` (any object when empty) and `group` (any group when empty). Returns void (no sret). `group`
// and `obj` are passed by value; `obj`'s ref is released on return.
void sslSYSTEM::DelayedFuncAction(DELACT action, dsSTRID group, sslOBJ_REF obj)
{
    // Debug scratch strings built by the original (their only effect is construct/destroy here).
    dsTSTRING<char> wildcard;
    wildcard.UnsafeInit("*", -1, 0);
    dsTSTRING<char> delayedSuffix;
    delayedSuffix.UnsafeInit(" (delayed)", -1, 0);

    int count = this->delayedFuncs.nElem;
    ++this->logFuncActive;

    dsSTRID anyGroup; // empty id == match every group

    for (int i = 0; i < count; ++i)
    {
        sslDELAYED_FUNCTION *df = this->delayedFuncs[i];

        if (obj.pObject && df->self.pObject != obj.pObject)
            continue;
        if (group.id != anyGroup.id && group.id != df->group.id)
            continue;

        bool clearRecord = false;
        switch (action)
        {
        case DELACT_PAUSE:
            ++df->pauseLocks;
            break;
        case DELACT_RESUME:
            if (--df->pauseLocks < 0)
                df->pauseLocks = 0;
            break;
        case DELACT_KILL:
            clearRecord = true;
            break;
        case DELACT_CALLNOW:
            df->timeLeft = 0.0f;
            df->pauseLocks = 0;
            df->Update(1.0f);
            clearRecord = true;
            break;
        }

        if (clearRecord)
        {
            sslOBJECT *pObject = df->self.pObject;
            if (pObject)
            {
                pObject->state.state &= ~8u; // drop the "delayed call pending" flag
                // ...but re-raise it if the object still owns another delayed call.
                for (int j = 0; j < this->delayedFuncs.nElem; ++j)
                {
                    if (this->delayedFuncs[j]->self.pObject == pObject)
                    {
                        pObject->state.state |= 8u;
                        break;
                    }
                }
            }
            this->delayedFuncs[i]->Clear();
        }
    }

    this->logPref.Clear();
    this->logPost.Clear();
    --this->logFuncActive;
}
