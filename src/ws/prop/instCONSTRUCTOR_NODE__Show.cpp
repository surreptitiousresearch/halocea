#include "../../headers/ws/prop/instCONSTRUCTOR_NODE.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/objFILTER_NAME.h"
#include "../../headers/ws/obj/obj_free_functions.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// instCONSTRUCTOR_NODE::Show(animINST*, bool, dsVECTOR<unsigned char,8>&) — 0x826E11A8
//
// ObjCB is the StateCondProc callback (boundary, obj/ObjCB — 0x826E1168): it marks
// selObjs[pObj->id] = 1 and returns true (always applies the state bit). pSelObj is the file-scope
// global it reads that selection vector through while the callback is in flight.
extern dsVECTOR<unsigned char, 8> *pSelObj;                 // ?pSelObj@@3PAV?$dsVECTOR@E$07@@A — boundary global
extern int ObjCB(objOBJ *pObj, void *usr);                  // boundary — StateCondProc, sets pSelObj[pObj->id]=1

void instCONSTRUCTOR_NODE::Show(animINST *pInst, bool show, dsVECTOR<unsigned char, 8> &selObjs)
{
    if (objId == -1) {
        // Resolve this node's object index by exact-name lookup, once, and cache it.
        objFILTER_NAME nameFilter(objName.CStr());
        objOBJ *foundObj = objFind(pInst->pObj, &nameFilter);
        objId = foundObj ? foundObj->id : -2;
    }

    if (objId >= 0 && objId < pInst->nObj && show) {
        // Deviation from decompiler: it modeled `pSelObj` sets/clears around the call as raw
        // stores to a global; reconstructed here as the same store/restore bracketing the
        // SetStateProc call, since ObjCB (invoked reentrantly from within it) reads pSelObj.
        pSelObj = &selObjs;
        pInst->ppObjList[objId]->SetStateProc(8, 0, ObjCB);
        pSelObj = nullptr;
    }
}
