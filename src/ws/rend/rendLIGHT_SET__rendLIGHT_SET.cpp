#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/rend/rendLIGHT_SET_LIST.h"
#include <string.h>

// ?m3dVUndef@@3Um3dV@@A — sentinel "undefined vector" constant used to poison fields that
// must be recomputed before first use (same convention as m3dVRand's use of m3dV helpers).
extern const m3dV m3dVUndef;

// rendLightSetsList — the single global registry of live light sets (rendLIGHT_SET_LIST).
extern "C" rendLIGHT_SET_LIST rendLightSetsList;

// rendLIGHT_SET::rendLIGHT_SET(animINST*, objOBJ*) @ 0x82ABDC70
//
// CAVEAT: the decompiler emits several redundant double-writes it did not fold (this->state.state
// set to 0 then immediately to 0x10000000; this->srData.pLock set to nullptr then, after the lock
// is initialized, to &this->lock) and, at the OnCreateLightSet call site, the same "extra phantom
// int parameters" register-pollution artifact already documented and stripped in
// rendLIGHT_SET_LIST::OnCreateLightSet.cpp (disasm there confirms the real ABI is just
// (this, pLS)). Both are reproduced/resolved the same way here: the redundant writes are kept
// verbatim (they are cheap store-store idioms, not bugs — matching disasm), and the call below
// passes just `this`. Additionally, the decompile's explicit shSH::shSH() calls on
// desiredStaticLightState/curStaticLightState's shColor.{shR,shG,shB} are the compiler's implicit
// member sub-object construction (shSH has a non-trivial default ctor) running before this body,
// not separate statements -- they are intentionally not reproduced as body statements below.
rendLIGHT_SET::rendLIGHT_SET(animINST *_pInstOwner, objOBJ *_pObjOwner)
{
    ref.refCount = 1;
    this->__vftable = (rendLIGHT_SET_vtbl *)&rendLIGHT_SET::vftable;

    srData.lampsData.pData = nullptr;
    srData.lampsData.nElem = 0;
    srData.lampsData.allocated = 0;
    srData.lampsData.__cl.line = 67;
    srData.lampsData.__cl.file = "func __console__ : *\n";

    srData.pLock = nullptr;
    srData.priorClass = 0;
    srData.priorCur = 0;
    srData.pos = m3dVUndef;

    // Packed 16-bit store in the original: {isInited=0, threadNmb=-1} written as one WORD
    // (big-endian PPC byte order), matching osLOCK's pre-Init() state.
    lock.isInited = false;
    lock.threadNmb = -1;
    lock.locker_file = nullptr;
    lock.locker_line = -1;
    lock.name = "rendLIGHT_SET";
    lock.Init();

    state.state = 0;
    pInstOwner = _pInstOwner;
    pObjOwner = _pObjOwner;
    frameUpdNmb = (unsigned int)-1;
    posUpdate = m3dVUndef;
    isOwnerInst = true;

    // desiredStaticLightState.shColor.{shR,shG,shB} are already default-constructed by the
    // implicit member-initialization that runs before this body (shSH() has a non-trivial
    // default ctor) -- the decompile's explicit shSH::shSH() calls on them are that automatic
    // sub-object construction, not separate source-level statements, so they are not repeated here.
    desiredStaticLightState.sdrCoeffs.ambient.r = 0.0f;
    desiredStaticLightState.sdrCoeffs.ambient.g = 0.0f;
    desiredStaticLightState.sdrCoeffs.ambient.b = 0.0f;
    desiredStaticLightState.sdrCoeffs.ambient.a = 0.0f;
    memset(&desiredStaticLightState, 0, sizeof(desiredStaticLightState));

    curStaticLightState.sdrCoeffs.ambient.r = 0.0f;
    curStaticLightState.sdrCoeffs.ambient.g = 0.0f;
    curStaticLightState.sdrCoeffs.ambient.b = 0.0f;
    curStaticLightState.sdrCoeffs.ambient.a = 0.0f;
    memset(&curStaticLightState, 0, sizeof(curStaticLightState));

    state.state = 0;
    state.state = 0x10000000;

    if (!pInstOwner)
        isOwnerInst = false;

    srData.pLock = &lock;

    rendLightSetsList.OnCreateLightSet(this);
}
