/* hcex_inst_create_clone @0x823CE4E8 — clone an existing animated instance onto the same template:
 * builds a create-data block named "clone_<prototype name>" with the prototype's averaged scale
 * (the mean of its x/y/z scale components, applied uniformly to all three axes), creates a new
 * instance from the prototype's template, and finishes it the same way hcex_inst_create does
 * (owner-notify + render pass id).
 *
 * DEVIATION: the decompiler passes 5 extra uninitialised register locals to
 * scnSCENE::CreateInst; the mangled callee (`?CreateInst@scnSCENE@@QAAPAVanimINST@@PAVanimTPL@@PAVanimCREATE_DATA@@@Z`)
 * and the disassembly (only `this`/pTpl/pCD are set up before the call) confirm it takes just the
 * template and create-data pointers — corrected here. */

#include "../headers/ws/anim/animCREATE_DATA.h"
#include "../headers/ws/anim/animINST.h"
#include "../headers/ws/scn/scnSCENE.h"
#include "../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../headers/ws/obj/objOBJ.h"

extern scnSCENE     *gsScenePtr;
extern gsLVL_SYSTEM *gsSysLevel;
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern "C" int sprintf_0(char *string, const char *format, ...);

animINST *hcex_inst_create_clone(animINST *pInstPrototype)
{
    animCREATE_DATA createData;

    float avgScale = (pInstPrototype->scale.z + pInstPrototype->scale.y + pInstPrototype->scale.x)
                    * 0.33333334f;
    createData.scaleX = avgScale;
    createData.scaleY = avgScale;
    createData.scaleZ = avgScale;

    const char *name = pInstPrototype->name;
    if (!name)
        name = empty_string;
    sprintf_0(createData.name, "clone_%s", name);

    if (!gsScenePtr)
        return 0;

    animINST *newInst = gsScenePtr->CreateInst(pInstPrototype->pTpl, &createData);
    if (!newInst)
        return 0;

    newInst->state2 |= 0x200000u;
    gsSysLevel->InstLoadNotify(newInst);
    newInst->pObj->SetRenderPassId(2);
    return newInst;
}
