#include "objOBJ.h"
#include "../anim/animINST.h"
#include "../m3d/m3dMATR.h"
#include "obj_free_functions.h"
// 0x82660938  ?objCalcNodeLTM@@YAXPAVobjOBJ@@@Z
// Recompute a single node's local-to-model matrix (matrLT), without recursing into children.
// Called by objCalcLTMGroup/objCalcLTMGroupExcl/objCalcLTM to do the actual per-node work:
//   - objOBJ::state bit 0x4 ("root/identity" node)         -> matrLT = identity
//   - has a parent                                          -> inherit or compose with parent->matrLT
//   - no parent, has an owning animINST                     -> inherit or compose with the
//                                                              instance's local-to-world matrix
//   - no parent, no owning instance                         -> matrLT = matrModel verbatim
// In the "has parent"/"has instance" cases, objOBJ::state bit 0x1000 selects "inherit the
// source matrix verbatim" vs "compose: matrModel * source -> matrLT" (m3dMultiplyMatr).
//
// CAVEAT: the decompiler represented each 64-byte m3dMATR struct assignment (matrLT = src)
// as a chain of 8 sequential 8-byte QWORD reads/writes threaded through HIDWORD/LODWORD
// pointer arithmetic over the overlapping elements[]/v[] union aliases (m3dMATR is a single
// 64-byte union — see m3dMATR.h). Verified by hand: each chain touches exactly 8 consecutive
// QWORD-sized slices covering the whole 64-byte matrix in order, so it is reproduced here as
// a plain struct assignment.

void objCalcNodeLTM(objOBJ *pObjFrom)
{
    int state = pObjFrom->state;

    if ((state & 4) != 0) {
        pObjFrom->matrLT = m3dMatrIdentity;
        return;
    }

    objOBJ *parent = pObjFrom->parent;
    if (parent) {
        if ((state & 0x1000) != 0)
            pObjFrom->matrLT = parent->matrLT;
        else
            m3dMultiplyMatr(&pObjFrom->matrModel, &parent->matrLT, &pObjFrom->matrLT);
        return;
    }

    animINST *pInst = pObjFrom->pInst;
    if (pInst) {
        if ((state & 0x1000) != 0) {
            pInst->_GetMatrL2W(&pObjFrom->matrLT);
        } else {
            m3dMATR instMatrL2W; // scratch: animINST's local-to-world matrix
            pInst->_GetMatrL2W(&instMatrL2W);
            m3dMultiplyMatr(&pObjFrom->matrModel, &instMatrL2W, &pObjFrom->matrLT);
        }
        return;
    }

    pObjFrom->matrLT = pObjFrom->matrModel;
}
