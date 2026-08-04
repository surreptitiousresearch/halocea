#include "objOBJ.h"
#include "../anim/animINST.h"
#include "../m3d/m3dMATR.h"
#include "obj_free_functions.h"
// 0x82A72030  ?_objValidateLTM@@YAXPAVobjOBJ@@@Z
// Internal recursive LTM validator used by objValidateLTM(). Walks up to the root via
// objOBJ::parent (validating ancestors first), then computes this node's matrLT:
//   - has a parent   -> inherit parent->matrLT verbatim, or compose matrModel * parent->matrLT
//                       (objOBJ::state bit 0x1000 selects which)
//   - at the root, has an owning animINST -> same inherit/compose choice against the
//                       instance's local-to-world matrix (animINST::_GetMatrL2W)
//   - at the root, no owning instance -> matrLT = matrModel verbatim
//
// CAVEAT: as in objCalcNodeLTM.cpp, the decompiler rendered each 64-byte m3dMATR struct
// assignment as a chain of 8 sequential QWORD reads/writes through HIDWORD/LODWORD pointer
// arithmetic over the elements[]/v[] union aliases; verified by hand to be full-matrix
// copies and reproduced here as plain struct assignments.

void _objValidateLTM(objOBJ *pObj)
{
    objOBJ *parent = pObj->parent;
    if (parent) {
        _objValidateLTM(parent);

        if ((pObj->state & 0x1000) != 0)
            pObj->matrLT = parent->matrLT;
        else
            m3dMultiplyMatr(&pObj->matrModel, &parent->matrLT, &pObj->matrLT);
        return;
    }

    animINST *pInst = pObj->pInst;
    if (pInst) {
        if ((pObj->state & 0x1000) != 0) {
            pInst->_GetMatrL2W(&pObj->matrLT);
        } else {
            m3dMATR instMatrL2W; // scratch: animINST's local-to-world matrix
            pInst->_GetMatrL2W(&instMatrL2W);
            m3dMultiplyMatr(&pObj->matrModel, &instMatrL2W, &pObj->matrLT);
        }
        return;
    }

    pObj->matrLT = pObj->matrModel;
}
