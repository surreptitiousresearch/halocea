#include "objOBJ.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// objOBJ::operator=(const objOBJ &src) @ 0x82661468
// Memberwise copy-assign every field except the affix string, which is copy-assigned by sharing
// (bump refcount) rather than deep copy: release this object's old affix buffer reference, then
// adopt (and bump the refcount of) the source's affix buffer.
objOBJ &objOBJ::operator=(const objOBJ &src)
{
    this->name = src.name;
    this->id = src.id;
    this->pGeom = src.pGeom;
    this->nFace = src.nFace;
    this->nVert = src.nVert;
    this->pInst = src.pInst;
    this->state = src.state;
    this->stateProc = src.stateProc;
    this->stateUsr64.state = src.stateUsr64.state;
    this->stateShare.state = src.stateShare.state;
    this->stateVis.state = src.stateVis.state;

    if (&src.affixes != &this->affixes) {
        dsTSTRING_BUF_HEADER<char> *oldAffixBuffer = this->affixes.str.pBuffer;
        if (--oldAffixBuffer->refCount == 0)
            dlFree(oldAffixBuffer);
        dsTSTRING_BUF_HEADER<char> *newAffixBuffer = src.affixes.str.pBuffer;
        this->affixes.str.pBuffer = newAffixBuffer;
        ++newAffixBuffer->refCount;
    }

    this->lastFameVisible = src.lastFameVisible;
    this->parent = src.parent;
    this->matrLT = src.matrLT;
    this->next = src.next;
    this->prev = src.prev;
    this->child = src.child;
    this->matrModel = src.matrModel;
    this->pMatrLTPrev = src.pMatrLTPrev;
    this->ps = src.ps;
    this->animNmb = src.animNmb;
    this->stateRend = src.stateRend;
    this->dbgRenderRefCount = src.dbgRenderRefCount;
    return *this;
}
