#pragma once
#include "objOBJ.h"
// ws-engine obj: free-function (non-member) part of the obj subsystem's public surface —
// node-hierarchy local-to-model ("LTM") matrix propagation, plus tree search/filter and
// pointer-list collection helpers. Declared together since no single class owns them.

struct objFILTER; // objFILTER.h — pointer only where a full definition isn't needed here

// 0x82B0B6F0 — recompute LTM for the group rooted at pObjFrom; recurses only into children
// with objOBJ::state bit 0x2 set. Skips the whole call if stateProc bit 0x800 (locked) is set.
void objCalcLTMGroup(objOBJ *pObjFrom);

// 0x82B0B780 — exclusive-locked variant of objCalcLTMGroup: always recurses into every child.
void objCalcLTMGroupExcl(objOBJ *pObjFrom);

// 0x82661040 — general (null-tolerant) LTM propagation entry point; skips nodes whose owning
// animINST has state2 bit 0x4 set. Always recurses into every child.
void objCalcLTM(objOBJ *pObjFrom);

// 0x82660938 — recompute a single node's matrLT from its state/parent/owning animINST,
// without recursing into children. Called by all three Group/LTM walkers above.
void objCalcNodeLTM(objOBJ *pObjFrom);

// 0x82A72390 — lazily validate a node's LTM chain (CDT-skeleton-driven or plain parent chain).
void objValidateLTM(objOBJ *pObj);

// 0x82A72030 — internal recursive LTM validator used by objValidateLTM(); walks to the root
// via objOBJ::parent, validating ancestors first, then computes this node's matrLT.
void _objValidateLTM(objOBJ *pObj);

// 0x82682B58 — find the first node named `name` in the subtree rooted at pObj (self included).
objOBJ *objFindName(objOBJ *pObj, const char *name);

// 0x826829C0 — depth-first search of the subtree rooted at pObj for the first node accepted
// by pObjFilter->IsUse(). Pre-order: pObj itself, then children in sibling-ring order.
objOBJ *objFind(objOBJ *pObj, objFILTER *pObjFilter);

// 0x826848D0 — collect up to maxNObj node pointers (starting at pObjParent) into objPtrList;
// returns the number of entries written. Thin wrapper over the recursive
// _objIncludePtrListID() helper (sibling function, external to this batch — boundary).
int objMakePtrListID(objOBJ *pObjParent, objOBJ **objPtrList, int maxNObj);

// 0x82682E68 (_objIncludePtrListID) — recursive collector used by objMakePtrListID(). boundary.
extern void objIncludePtrListID(objOBJ *pObjParent, objOBJ **objPtrList, int maxNObj, int *pCount);

// ?m3dMatrIdentity@@3Vm3dMATR@@A — global identity matrix constant. boundary (data, m3d subsystem).
struct m3dMATR;
extern m3dMATR m3dMatrIdentity;
