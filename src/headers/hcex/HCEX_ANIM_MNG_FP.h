#pragma once
#include "../ws/anim/animMNG.h"
#include "hcex_inst_sync_data.h"
// HCEX bridge: first-person-weapon animation manager — an animMNG subclass whose ApplyAnim
// override re-syncs each fpModels entry's node matrices from the Blam-side skeleton before
// delegating back to the base engine's anim application. DB-verified layout (types_members
// HCEX_ANIM_MNG_FP): base animMNG @0 (472 bytes), sync_data@472 (hcex_inst_sync_data, 3392 bytes)
// — size 3864.

struct fioFILE;
struct animSEQ;
struct animSEQ_PROPS;
struct animOBJ_ANIM;
struct animTRAN_INFO;
struct animMNG_UPDATE_INFO;
struct animUPDATE_STATUS;
struct animLAYER_DESC_BASE;
struct objOBJ;
namespace anim_blend { struct SEQ_ID_FULL; }

// Full 22-slot vtbl per DB types_members HCEX_ANIM_MNG_FP_vtbl.
typedef struct HCEX_ANIM_MNG_FP_vtbl {
    void (*dtr_HCEX_ANIM_MNG_FP)(struct HCEX_ANIM_MNG_FP *self, int freeMemory); // 0x00 deleting dtor
    int  (*Init)(struct HCEX_ANIM_MNG_FP *self, const dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_DESC_BASE, 0, Deleter<animLAYER_DESC_BASE> > >, 8> *layers); // 0x04
    void (*Read)(struct HCEX_ANIM_MNG_FP *self, fioFILE *file);   // 0x08
    void (*Write)(struct HCEX_ANIM_MNG_FP *self, fioFILE *file);  // 0x0C
    void (*NotifyChangeLOD)(struct HCEX_ANIM_MNG_FP *self, float lod); // 0x10
    int  (*GetNAnimSeq)(struct HCEX_ANIM_MNG_FP *self);           // 0x14
    anim_blend::SEQ_ID_FULL *(*GetAnimSeq)(struct HCEX_ANIM_MNG_FP *self, anim_blend::SEQ_ID_FULL *result, const dsTSTRING<char> *name); // 0x18
    animSEQ *(*GetAnimSeq_2)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *id); // 0x1C
    animSEQ *(*GetAnimSeq_3)(struct HCEX_ANIM_MNG_FP *self, int seqNmb); // 0x20
    const animSEQ_PROPS *(*GetSeqInfo)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *id); // 0x24
    void (*FillAnimSeqStaticRef)(struct HCEX_ANIM_MNG_FP *self, const dsTSTRING<char> *name, anim_blend::SEQ_ID_FULL *id); // 0x28
    const animOBJ_ANIM *(*GetObjAnim)(struct HCEX_ANIM_MNG_FP *self, objOBJ *obj, animSEQ *seq); // 0x2C
    void (*EvalTranData)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *from, const anim_blend::SEQ_ID_FULL *to, animTRAN_INFO *tranInfo); // 0x30
    void (*UpdateAnim)(struct HCEX_ANIM_MNG_FP *self, const animMNG_UPDATE_INFO *updateInfo, animUPDATE_STATUS *status); // 0x34
    void (*ApplyAnim)(struct HCEX_ANIM_MNG_FP *self, unsigned __int8 typeValidation); // 0x38
    void (*AddRefAnimSeq)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *id); // 0x3C
    void (*RemoveRefAnimSeq)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *id); // 0x40
    void (*EnableAnimLink)(struct HCEX_ANIM_MNG_FP *self);        // 0x44
    void (*DisableAnimLink)(struct HCEX_ANIM_MNG_FP *self);       // 0x48
    float (*GetSeqPlayRate)(struct HCEX_ANIM_MNG_FP *self, const anim_blend::SEQ_ID_FULL *id); // 0x4C
    void (*SetupAnimLayers)(struct HCEX_ANIM_MNG_FP *self, const dsVECTOR<dsPAIR<dsSTRID, dsSHARED_PTR<animLAYER_DESC_BASE, 0, Deleter<animLAYER_DESC_BASE> > >, 8> *layers); // 0x50
    int  (*HasAnimData)(struct HCEX_ANIM_MNG_FP *self);           // 0x54
} HCEX_ANIM_MNG_FP_vtbl;

struct HCEX_ANIM_MNG_FP : animMNG {
    static HCEX_ANIM_MNG_FP_vtbl vftable; // `HCEX_ANIM_MNG_FP::`vftable'' — boundary, not decompiled
    hcex_inst_sync_data sync_data;        // 0x1D8

    // 0x823CE220 — delegate to animMNG(pInst, nullptr), then install this class's vftable.
    HCEX_ANIM_MNG_FP(animINST *pInst);

    // 0x823CE290 — thunk straight to ~animMNG().
    ~HCEX_ANIM_MNG_FP(); // base dtor is not a C++ virtual (opaque boundary): no 'override'

    // 0x823D0C14 — compiler-generated vector deleting destructor (`??_E` mangle).
    HCEX_ANIM_MNG_FP *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823DF1E4 (overrides animMNG::ApplyAnim) — locate this manager's entry in the global
    // fpModels vector by matching pInst; if its resolved HCEX_MODEL has per-node bridge matrices,
    // recompute each node's model matrix from the synced Blam node transform (inverse of the
    // parent's local transform composed with the Blam node matrix, scale removed), then
    // recalculate every node's LTM. On first call, latches the static fpOfs/rotX/Y/Z reposition
    // once (guarded lazy-init); when `repos` is set, applies that fixed offset/rotation to the
    // instance and recalculates the LTMs again.
    void ApplyAnim(unsigned char typeValidation); // base slot is a vtable fn-ptr, not a C++ virtual: no 'override'
};
