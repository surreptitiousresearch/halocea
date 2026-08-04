#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "animLAYER_BASE.h"
#include "anim_blend_SEQ_INFO.h"
#include "animAF_LIST.h"
// ws-engine anim: per-instance animation manager. DB-verified layout
// (types_members animMNG): __vftable@0, pInst@4, pASys@8, layerList@12,
// listLayerVisitExcluded@32, afList@52 — total size 472 (0x1D8). The vtable, the owning
// instance, the blend-layer list, the command entry points, and the action-frame list are
// modeled.

#include "../ds/dsTSTRING.h"

struct fioFILE;           // fio/fioFILE.h — file stream (pointer only)
struct animINST;          // animINST.h — owning animated instance (pointer only)
struct animSYSTEM;        // anim subsystem — owning animation system (pointer only)  boundary
struct animMNG;           // forward decl for the vtbl below (pointer only)

struct animSEQ;
struct animSEQ_PROPS;
struct animOBJ_ANIM;
struct animTRAN_INFO;
struct animMNG_UPDATE_INFO;
struct animUPDATE_STATUS;
struct animLAYER_DESC_BASE;
struct objOBJ;
namespace anim_blend { struct SEQ_ID_FULL; }

// Full slot list per DB (types_members animMNG_vtbl). boundary — slots not decompiled here.
typedef struct animMNG_vtbl {
    void (*dtr_animMNG)(animMNG *self, int freeMemory); // 0x00 deleting dtor; freeMemory!=0 also operator-deletes self
    int  (*Init)(animMNG *self, const dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_DESC_BASE, 0, Deleter<animLAYER_DESC_BASE> > >, 8> *descList); // 0x04
    void (*Read)(animMNG *self, fioFILE *file);  // 0x08
    void (*Write)(animMNG *self, fioFILE *file); // 0x0C
    void (*NotifyChangeLOD)(animMNG *self, float lod); // 0x10
    int  (*GetNAnimSeq)(animMNG *self); // 0x14
    anim_blend::SEQ_ID_FULL *(*GetAnimSeq)(animMNG *self, anim_blend::SEQ_ID_FULL *result, const dsTSTRING<char> *name); // 0x18
    animSEQ *(*GetAnimSeq_2)(animMNG *self, const anim_blend::SEQ_ID_FULL *id); // 0x1C
    animSEQ *(*GetAnimSeq_3)(animMNG *self, int seqNmb); // 0x20
    const animSEQ_PROPS *(*GetSeqInfo)(animMNG *self, const anim_blend::SEQ_ID_FULL *id); // 0x24
    void (*FillAnimSeqStaticRef)(animMNG *self, const dsTSTRING<char> *name, anim_blend::SEQ_ID_FULL *id); // 0x28
    const animOBJ_ANIM *(*GetObjAnim)(animMNG *self, objOBJ *obj, animSEQ *seq); // 0x2C
    void (*EvalTranData)(animMNG *self, const anim_blend::SEQ_ID_FULL *from, const anim_blend::SEQ_ID_FULL *to, animTRAN_INFO *info); // 0x30
    void (*UpdateAnim)(animMNG *self, const animMNG_UPDATE_INFO *info, animUPDATE_STATUS *status); // 0x34
    void (*ApplyAnim)(animMNG *self, unsigned __int8 applyToChildren); // 0x38
    void (*AddRefAnimSeq)(animMNG *self, const anim_blend::SEQ_ID_FULL *id); // 0x3C
    void (*RemoveRefAnimSeq)(animMNG *self, const anim_blend::SEQ_ID_FULL *id); // 0x40
    void (*EnableAnimLink)(animMNG *self); // 0x44
    void (*DisableAnimLink)(animMNG *self); // 0x48
    float (*GetSeqPlayRate)(animMNG *self, const anim_blend::SEQ_ID_FULL *id); // 0x4C
    void (*SetupAnimLayers)(animMNG *self, const dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_DESC_BASE, 0, Deleter<animLAYER_DESC_BASE> > >, 8> *descList); // 0x50
    int  (*HasAnimData)(animMNG *self); // 0x54
} animMNG_vtbl;

typedef struct animMNG {
    // One blend layer: a string key paired with a shared handle to the layer object.
    typedef dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > > LAYER_ENTRY;

    static animMNG_vtbl vftable; // `animMNG::`vftable'' -- boundary, not decompiled
    animMNG_vtbl  *__vftable;              // 0x00
    animINST      *pInst;                  // 0x04 instance this manager drives
    animSYSTEM    *pASys;                  // 0x08 owning animation system
    dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_BASE, 0, Deleter<animLAYER_BASE> > >, 8> layerList; // 0x0C active blend layers (key -> layer)
    dsVECTOR<short, 8>        listLayerVisitExcluded; // 0x20 layer ids skipped during a visit
    animAF_LIST    afList;                  // 0x34

    // 0x8267E9B8 — link this manager to its owning instance/system and default-construct the
    // layer lists and action-frame list.
    animMNG(animINST *_pInst, animSYSTEM *animSys);

    // Virtual (vtbl slot +0x38): apply the manager's current animation to its instance.
    // `applyToChildren` is the literal 1 the caller passes. boundary — not decompiled here.
    void ApplyAnim(int applyToChildren); // boundary (virtual)

    // Number of animation sequences available to this manager (vtbl slot +0x14). boundary (virtual).
    int GetNAnimSeq(); // boundary (virtual)

    // Resolve a sequence by index (vtbl slot +0x20); nullptr if absent. boundary (virtual).
    animSEQ *GetAnimSeq(int seqNmb); // boundary (virtual)

    // 0x8267D8E0 — lock the instance, dispatch `cmd`, and invalidate the pose if it changed.
    void SendAnimCmd(const animCMD_BASE &cmd);

    // 0x8267C338 — dispatch `cmd` to the target layer(s) without taking the per-instance lock,
    // reporting whether the pose changed via `status`.
    void SendAnimCmdNoLock(const animCMD_BASE &cmd, animUPDATE_STATUS *status);

    // 0x8267C430 — the sequence currently playing on layer `layerId` (sret). Returns {-1,-1}
    // when the layer is empty or its sequence is not valid.
    anim_blend::SEQ_ID_FULL GetCurAnimSeq(int layerId);

    // Resolve a fully-qualified sequence id to its animSEQ (nullptr if absent).  boundary
    // (called by animINST::ValidateOBB).
    animSEQ *GetAnimSeq_2(const anim_blend::SEQ_ID_FULL *id);
} animMNG;
