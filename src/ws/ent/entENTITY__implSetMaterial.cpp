#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entENTITY_showhide_boundary.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objITER_TREE.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/mtl/MTL_DESC_DB.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

#include <cstring>

// entENTITY::implSetMaterial @ 0x826A00F8 (virtual) — apply a material override to one object of
// this entity's model, then flag it as overridden.
//
// NAMING: the header declares the parameters (materialName, materialValue); the disassembly shows
// the first argument is the material to bind and the second selects the object node by name. Named
// here per the observed usage (mtlName, objName) to match the body.
//
// When objName is empty, the whole model root (pInst->pObj) is targeted. Otherwise the model tree
// is walked (pre-order) for the first faced node whose name matches objName. On a hit, the material
// is selected by name (or restored to default when mtlName is empty) and the node's render-state
// override bit (0x100) is set.
void entENTITY::implSetMaterial(const dsTSTRING<char> &mtlName, const dsTSTRING<char> &objName)
{
    objOBJ *target = nullptr;

    if (objName.pBuffer->strLen == 0)
    {
        target = this->pInst->pObj;
    }
    else
    {
        objOBJ *root = this->pInst->pObj;
        objITER_TREE it;
        it.pRoot = root;
        it.pCur = root;
        it.isSkipped = 0;

        while (it.pCur)
        {
            objOBJ *cur = it.pCur;
            if (cur->nVert > 0)
            {
                const char *nodeName = cur->name ? cur->name : empty_string;
                if (strcmp(objName.pBuffer->str, nodeName) == 0)
                {
                    target = cur;
                    break;
                }
            }
            it.Next();
        }
    }

    if (target)
    {
        if (mtlName.pBuffer->strLen)
            mtlManager->SelectMaterialsByMtlName(target, mtlName);
        else
            mtlManager->SelectMaterialsByDefault(target);

        target->SetStateRendNo(256);
    }
}
