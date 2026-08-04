#include "objOBJ.h"

// objOBJ::objOBJ() @ 0x82661A50 (protected — only objOBJ and derived classes construct directly)
// Default-construct a blank object: zero every state slot (state/stateProc/stateRend and the
// 64-bit stateUsr64/stateShare/stateVis holders), an empty sibling/child list (self-linked
// next/prev, null child/parent), default-construct affixes and ps, and reset the transform
// matrices to identity.
// CAVEAT: the decompile builds the three 64-bit `state` fields via a single zeroed __int64
// scratch local, reused for all three assigns — the same zero-init idiom already documented for
// these fields; each is a plain `= 0` here, not a shared temp. The decompile also
// shows explicit dsAFFIX_STRING::dsAFFIX_STRING(&affixes) / psSECTION::psSECTION(&ps) calls
// interleaved with the field stores below — these are the compiler's ordinary member default-
// construction (affixes/ps run their own default ctors automatically as objOBJ members) and are
// not written out here.
objOBJ::objOBJ()
{
    this->stateUsr64.state = 0;
    this->stateShare.state = 0;
    this->stateVis.state = 0;
    this->next = this;
    this->prev = this;
    this->dbgRenderRefCount = 0;
    this->state = 0;
    this->stateProc = 0;
    this->stateRend = 0;
    this->id = 0;
    this->name = nullptr;
    this->nFace = 0;
    this->nVert = 0;
    this->parent = nullptr;
    this->child = nullptr;
    this->pInst = nullptr;
    this->animNmb = -1;
    this->pGeom = nullptr;
    this->pMatrLTPrev = nullptr;
    this->lastFameVisible = 0;
    this->matrModel.Identity();
    this->matrLT.Identity();
}
