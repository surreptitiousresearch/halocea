#include "animCREATE_DATA.h"
#include <cstring>

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free
void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug-tagged new (Saber)
void operator delete(void *ptr); // boundary — CRT operator delete
extern const m3dV m3dVUndef; // boundary — sentinel/undefined vector constant

// animCREATE_DATA::operator=(const animCREATE_DATA&) @ 0x8252EF60
// Memberwise copy: transform matrix, frameCur/state, ps, affixes (shared-buffer assign), name
// (NUL-terminated string), animSeqNmb/animSeqIsCycled/isIgnorePS_Scale, and scaleX/Y/Z. Then
// reconcile srData: if the source has shader-replacement data, deep-copy it (allocating a new
// record on this side if it doesn't have one yet); otherwise free this side's srData.
// CAVEAT: the decompile packs `&src->ps` behind a stray `_QWORD` scratch (`v4`) that is also used
// for the preceding matrInst row copy — a register-reuse artifact, not a real aliasing of ps with
// matrix data. Reconstructed as the plain `&src->ps` the call obviously intends. The per-QWORD
// matrInst copy is likewise collapsed to a single struct assignment (matrInst is a plain 64-byte
// POD m3dMATR), and the byte-at-a-time `name` copy loop is collapsed to strcpy. Also NOTE: the
// disasm's allocation-failure fallback (operator new returns null) falls through to call
// rendSR_DATA::operator= on a just-nulled this->srData -- reproduced verbatim below since the DB
// is ground truth for control flow, even though it is a latent null-deref in the original game.
animCREATE_DATA &animCREATE_DATA::operator=(const animCREATE_DATA &src)
{
    if (this == &src)
        return *this;

    this->matrInst = src.matrInst;
    this->frameCur = src.frameCur;
    this->state = src.state;
    this->ps = src.ps;
    this->affixes = src.affixes;
    strcpy(this->name, src.name);
    this->animSeqNmb = src.animSeqNmb;
    this->animSeqIsCycled = src.animSeqIsCycled;
    this->isIgnorePS_Scale = src.isIgnorePS_Scale;
    this->scaleX = src.scaleX;
    this->scaleY = src.scaleY;
    this->scaleZ = src.scaleZ;

    if (src.srData)
    {
        if (!this->srData)
        {
            rendSR_DATA *newSrData = (rendSR_DATA *)operator new(sizeof(rendSR_DATA),
                "D:\\Projects\\code\\common\\src.sys\\animation\\anim_anim.cpp", 0x1C1u);
            if (newSrData)
            {
                newSrData->lampsData.pData = nullptr;
                newSrData->lampsData.nElem = 0;
                newSrData->lampsData.allocated = 0;
                newSrData->lampsData.__cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\rend_light_set.h";
                newSrData->lampsData.__cl.line = 67;
                newSrData->pLock = nullptr;
                newSrData->priorClass = 0;
                newSrData->priorCur = 0;
                newSrData->pos = m3dVUndef;
                this->srData = newSrData;
                *this->srData = *src.srData;
                return *this;
            }
            this->srData = nullptr;
        }
        // Reproduced verbatim (see CAVEAT above): reached only via the allocation-failure path,
        // where this->srData was just set to nullptr.
        *this->srData = *src.srData;
        return *this;
    }

    if (this->srData)
    {
        dlFree(this->srData->lampsData.pData);
        operator delete(this->srData);
    }
    this->srData = nullptr;
    return *this;
}
