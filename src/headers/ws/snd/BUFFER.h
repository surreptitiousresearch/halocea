#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine snd: abstract playing-sound handle (D:\Projects\code\common\src.sys\drv\sound\snd.cpp).
// DB-verified layout (types_members snd::BUFFER): __vftable@0, state@4 (dsFLAGS<BUFFER_STATE,int>),
// name@8 (dsSTRID), categoryId@12 (dsSTRID), dbgInfo@16 (dsTSTRING<char>), delay@20 (float),
// delayTimer@24 (float), refCount@28 (int) — size 32.
// Concrete backends (e.g. snd::BUFFER_FMOD) derive from this and fill in BUFFER_vtbl; those
// derived classes and the FMOD backend are OTHER-SUBSYSTEM boundaries, not decompiled here.

// State bits (types_enum_values snd::BUFFER_STATE).
enum snd_BUFFER_STATE {
    BST_INITED = 1,
    BST_PLAYING = 2,
    BST_PAUSED = 4,
    BST_STOPPED = 8,
    BST_3D = 16,
    BST_LOOPED = 32,
    BST_VOICEOVER = 64,
    BST_VIRTUAL = 128,
    BST_DELAY = 256,
    BST_HAS_BEEN_STARTED = 512,
    BST_STREAMED = 1024,
    BST_USER = 2048,
};

namespace snd {

// Re-exported under its DB name so dsFLAGS<snd::BUFFER_STATE,int> matches the mangled type.
typedef snd_BUFFER_STATE BUFFER_STATE;

struct NOTIFICATION_INFO; // boundary — external to this batch

struct BUFFER; // fwd for the vtbl's `this` parameters

// DB-verified layout (types_members snd::BUFFER_vtbl): 20 slots, each a plain function pointer
// (no RTTI/offset-to-top columns preceding slot 0 in this table dump).
typedef struct BUFFER_vtbl {
    void (__fastcall *Play)(BUFFER *self);
    void (__fastcall *Pause)(BUFFER *self, bool pause);
    void (__fastcall *Stop)(BUFFER *self, bool immediate);
    bool (__fastcall *Set3DPanLevel)(BUFFER *self, float panLevel);
    bool (__fastcall *KeyOff)(BUFFER *self);
    bool (__fastcall *SetRPCParameter)(BUFFER *self, const char *name, float value);
    bool (__fastcall *IsStarving)(BUFFER *self);
    int (__fastcall *GetTime)(BUFFER *self);
    int (__fastcall *GetCurTime)(BUFFER *self);
    float (__fastcall *GetVolume)(BUFFER *self);
    float (__fastcall *GetPitch)(BUFFER *self);
    void (__fastcall *SetVolume)(BUFFER *self, float volume);
    void (__fastcall *SetPitch)(BUFFER *self, float pitch);
    void (__fastcall *SetOcclusion)(BUFFER *self, float directOcclusion, float reverbOcclusion);
    void (__fastcall *GetOcclusion)(BUFFER *self, float *directOcclusion, float *reverbOcclusion);
    void (__fastcall *SetPosVel)(BUFFER *self, const m3dV *pos, const m3dV *vel);
    void (__fastcall *GetPosVel)(BUFFER *self, m3dV *pos, m3dV *vel);
    void (__fastcall *Update)(BUFFER *self, float deltaTime);
    void (__fastcall *dtr_BUFFER)(BUFFER *self, int deleteFlag); // scalar-deleting dtr slot; deleteFlag bit0 => free
    void (__fastcall *Term)(BUFFER *self);
} BUFFER_vtbl;

typedef struct BUFFER {
    BUFFER_vtbl *__vftable;              // 0x00
    dsFLAGS<BUFFER_STATE, int> state;    // 0x04
    dsSTRID name;                        // 0x08
    dsSTRID categoryId;                  // 0x0C
    dsTSTRING<char> dbgInfo;             // 0x10
    float delay;                         // 0x14
    float delayTimer;                    // 0x18
    int refCount;                        // 0x1C — released via osLockedDecrement, not freed here

    // 0x82696C48 — release a reference; asserts refCount stayed non-negative, then atomically
    // decrements it. Does NOT free/dispose the buffer itself (that is the caller/pool's job once
    // the count reaches zero) — this method only ever decrements.
    void Release();

    // 0x82696C?? — atomically bump the reference count (mirror of Release). boundary decl only:
    // its body lives in the src/ws/snd drain; declared here for snd::SYSTEM_FMOD::GetBuffer.
    void AddRef();

    // 0x82697150 — base implementation of the virtual Play() slot. Asserts the buffer is not
    // stopped; if a start delay is armed, latch delayTimer and enqueue onto snd::System's
    // needToUpdate list; otherwise clear the pending-delay bit and start immediately.
    void Play();
} BUFFER;

} // namespace snd
