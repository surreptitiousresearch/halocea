/* ?SetRelations@instCONSTRUCTOR_NODE@@QAAXPAVanimINST@@PAUinstCONSTRUCTOR_NODE_RELATIONS@@@Z @0x826E1F98 */
#include "../../headers/ws/prop/instCONSTRUCTOR_NODE.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// instCONSTRUCTOR_NODE::SetRelations(animINST*, instCONSTRUCTOR_NODE_RELATIONS*) — 0x826E1F98
//
// Walk up the parent chain from this node to the nearest ancestor with a valid `objId`. If one
// exists, look up a named RELATION override (by ancestor/child object name) in `rels`; if found,
// recompute this node's `transofrm` so that re-applying it (relative to the ancestor's world
// transform) reproduces the override's rotate/translate/scale exactly, and stamp the override
// straight onto the child object's current local transform too. No-op if this node's own `objId`
// is invalid, `rels` is null, or no ancestor has a valid `objId`.
void instCONSTRUCTOR_NODE::SetRelations(animINST *pInst, instCONSTRUCTOR_NODE_RELATIONS *rels)
{
    if (objId < 0 || !rels)
        return;

    objOBJ **objList = pInst->ppObjList;
    objOBJ *childObj = objList[objId];

    // Deviation from decompiler: only the FIRST ancestor with a valid `objId` is ever processed
    // (the original loop unconditionally `return`s once it finds one, whether or not a RELATION
    // match exists) — reconstructed here as `continue`-past-invalid then `return` after handling
    // the first valid one.
    for (instCONSTRUCTOR_NODE *ancestor = parent; ancestor; ancestor = ancestor->parent)
    {
        if (ancestor->objId < 0)
            continue;

        objOBJ *ancestorObj = objList[ancestor->objId];

        dsTSTRING<char> childName;
        childName.UnsafeInit(childObj->name ? childObj->name : empty_string, -1, 0);

        dsTSTRING<char> parentName;
        parentName.UnsafeInit(ancestorObj->name ? ancestorObj->name : empty_string, -1, 0);

        instCONSTRUCTOR_NODE_RELATIONS::RELATION *relation = rels->GetRelation(parentName, childName);

        if (relation)
        {
            // Original child local transform, expressed in the ancestor's local space.
            m3dMATR origChildLT = childObj->matrLT;
            m3dMATR ancestorTransform = ancestor->transofrm;
            m3dMATR childInAncestorSpace;
            m3dMultiplyMatr(&origChildLT, &ancestorTransform, &childInAncestorSpace);

            // Build the RELATION override transform: identity, then rotate/translate/scale.
            m3dMATR overrideTransform;
            overrideTransform.Identity();
            overrideTransform.MakeEuler(relation->rotation.x, relation->rotation.y, relation->rotation.z);
            overrideTransform.Translate(&relation->translation, 2);
            // Deviation from decompiler: it fabricated a phantom, never-assigned 4th `oper` arg
            // here (uninitialized local) -- disasm confirms r7=2 is loaded immediately before
            // this call, the same `oper` value Translate above used.
            overrideTransform.Scale(relation->scale.x, relation->scale.y, relation->scale.z, 2);

            // combined = childInAncestorSpace * overrideTransform
            m3dMATR combined;
            m3dMultiplyMatr(&childInAncestorSpace, &overrideTransform, &combined);

            // Deviation from decompiler: it rendered this struct copy as eight manual 8-byte
            // stores and mis-decoded the next instruction as
            // `m3dMATR::Identity((m3dMATR*)LODWORD(combined.elements[14]))` (a garbage pointer
            // cast of a float's raw bits). Disasm shows it is really a plain 64-byte struct copy
            // followed by a fresh, separately-identitied scratch matrix -- reconstructed below.
            childObj->matrLT = combined;

            // inverseScratch starts as identity (safe fallback if childInAncestorSpace turns out
            // singular), then Invert overwrites it with childInAncestorSpace's inverse.
            m3dMATR inverseScratch;
            inverseScratch.Identity();
            childInAncestorSpace.Invert(&inverseScratch);

            // this->transofrm = inverse(childInAncestorSpace) * combined
            m3dMultiplyMatr(&inverseScratch, &combined, &transofrm);
        }

        return;
    }
}
