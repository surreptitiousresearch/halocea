#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // extern aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/smt/smtSMART_OBJ_BASE.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/apCL.h"
#include <string.h>

// aiBRAIN::CheckCallForceAF_Smt(const dsVECTOR<dsDATA,8>&) @ 0x832677D0
// ?CheckCallForceAF_Smt@aiBRAIN@@...
//
// SSL guard for a "ForceAF smart" call: unbox the smart-object name from funcParams[0], look it up
// in the planner, enumerate its available entry-point slots and ask the body whether it can use any.
// Returns true (allow) unless the object exists AND has slots AND the body can use none of them.

extern "C" void dlFree(void *ptr);

bool aiBRAIN::CheckCallForceAF_Smt(const dsVECTOR<dsDATA, 8> &funcParams)
{
    dsTSTRING<char> smtName; // binary: UnsafeInitEmpty — the shared empty string
    bool result = true;

    // (strong-assert idx 0 < nElem folds into operator[])
    if (funcParams[0].GetValue<dsTSTRING<char> >(smtName, DSD_CONV_RETRIEVE))
    {
        dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj =
            aiPlanner->FindSmartObject(smtName);
        if (smtObj.pointee)
        {
            dsVECTOR<smtSMT_ENTER_INFO, 8> slots;
            memset(&slots, 0, 12);
            apCL cl;
            cl.line = 193;
            cl.file = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_smart.cpp";
            slots.__cl = cl;
            smtObj.pointee->GetAvailableSlotsInfo(
                &slots, 1,
                (void *)"D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8_smart.cpp");

            if (slots.nElem)
            {
                bool anyUsable = false;
                for (int i = 0; i < slots.nElem; ++i)
                {
                    smtSMT_ENTER_INFO &slot = slots[i];
                    if (this->body.pointee->CanUseSmart(&smtObj, &slot.info))
                    {
                        anyUsable = true;
                        break;
                    }
                }
                // Usable slot found -> allow (1); none usable -> reject (0).
                result = anyUsable;
            }

            dlFree(slots.pData); // raw free (mirrors the decompiler; bypasses element dtors)
            slots.pData = nullptr;
            smtObj.Reset();
        }
    }

    return result; // smtName's dsTSTRING dtor releases the shared empty buffer at scope exit
}
