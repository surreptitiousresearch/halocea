#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/prop/propSMT.h"
#include "../../headers/animINST.h"

extern void _apLog(const char *fmt, ...);

// scnSCENE::FindInstName(gsScenePtr, name): resolve a scene instance by name. Hoisted to a
// boundary (scene subsystem) so this TU need not model scnSCENE/gsScenePtr.  boundary
extern animINST *scnFindInstName(const char *name);

// Locate the prop_smt property on `ent`, whether the entity IS a prop_smt or hosts one inside its
// property container (the propBASE/propCONTAINER/dsTYPE_ID::IsDerivedFrom traversal at
// 0x83168438-0x831684FC). Hoisted to the prop subsystem boundary (next frontier).  boundary
extern propSMT *entFindPropSMT(entENTITY *ent);

// aiPLANNER::FindSmartObject(const dsTSTRING<char>&) — 0x831683D0
// Resolve a smart-object by scene-instance name and return its shared pointer (empty on failure).
dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >
aiPLANNER::FindSmartObject(const dsTSTRING<char> &nameInstSmt)
{
    animINST *inst = scnFindInstName(nameInstSmt.pBuffer->str);
    if (!inst)
    {
        _apLog("~AI,Error~ FindSmartObject(\"%s\"): can't find specified entity",
               nameInstSmt.pBuffer->str);
        return dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >();
    }

    propSMT *smt = entFindPropSMT(inst->pEnt);
    if (!smt)
    {
        _apLog("~AI,Error~ FindSmartObject(\"%s\"): no prop_smt property on specified entity",
               nameInstSmt.pBuffer->str);
        return dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >();
    }

    return smt->smtObj;
}
