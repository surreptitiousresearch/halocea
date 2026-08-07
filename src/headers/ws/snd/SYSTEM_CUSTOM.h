#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine snd: an abstract, project-supplied sound system override (distinct root hierarchy
// from snd::SYSTEM -- its own vtable, not derived from SYSTEM). HCEX's global `SystemCustom`
// points at a HALO_SOUND_SYSTEM instance at runtime. BOUNDARY -- only the vtable shape touched by
// HALO_MNG::ProcessMsg/ProcessTERM_LEVEL is modeled; not decompiled as a concrete class here.
// DB-verified layout (types_members snd::SYSTEM_CUSTOM_vtbl): Init@0, Term@4, Update@8,
// InitLevel@12, TermLevel@16, DbgRenderSounds@20, destructor@24.
// DB-verified (types_members snd::SYSTEM_CUSTOM): __vftable@0 -- size 4.
// Sole home for snd::HRTF / snd::INIT / snd::SYSTEM_CUSTOM / SYSTEM_CUSTOM_vtbl; snd_fmod_boundary.h
// carried a narrower second copy of the first three (INIT without its 0x03 pad, an unsized HRTF,
// SYSTEM_CUSTOM with only a forward-declared vtbl) and now includes this header.
// Related binary symbols: ??_7SYSTEM_CUSTOM@snd@@6B@ @0x82111408 (the vtable object the __vftable
// slot points at), ??0SYSTEM_CUSTOM@snd@@QAA@XZ @0x836B5570, ??1SYSTEM_CUSTOM@snd@@UAA@XZ
// @0x836B4D70 -- all boundary, owned by the src/ws/snd/ drain.

namespace snd {

// DB types_enum_values snd::HRTF.
enum HRTF : int {
    HRTF_NONE  = 0,
    HRTF_LIGHT = 1,
    HRTF_FULL  = 2,
};

// DB types_members snd::INIT -- init parameter block (a struct, not an enum), size 8.
typedef struct INIT {
    bool waveOutput; // 0x00
    bool enableEax;  // 0x01
    bool enableHW;   // 0x02
    char _pad03[1];  // 0x03
    HRTF hrtf;       // 0x04
} INIT;

struct SYSTEM_CUSTOM;

typedef struct SYSTEM_CUSTOM_vtbl { // DB types_members snd::SYSTEM_CUSTOM_vtbl
    bool (*Init)(SYSTEM_CUSTOM *self, INIT initParams);                    // 0x00
    void (*Term)(SYSTEM_CUSTOM *self);                                     // 0x04
    void (*Update)(SYSTEM_CUSTOM *self);                                   // 0x08
    void (*InitLevel)(SYSTEM_CUSTOM *self, const dsTSTRING<char> *);      // 0x0C
    void (*TermLevel)(SYSTEM_CUSTOM *self);                                // 0x10
    void (*DbgRenderSounds)(SYSTEM_CUSTOM *self);                          // 0x14
    void (*dtr_SYSTEM_CUSTOM)(SYSTEM_CUSTOM *self, int deleteFlag);                        // 0x18 — deleting dtor: vftable+0x18 holds ??_GSYSTEM_CUSTOM@snd@@UAAPAXI@Z
} SYSTEM_CUSTOM_vtbl;

typedef struct SYSTEM_CUSTOM {
    SYSTEM_CUSTOM_vtbl *__vftable; // 0x00

    // `snd::SYSTEM_CUSTOM::`vftable'' — boundary. ??_7SYSTEM_CUSTOM@snd@@6B@ @0x82111408; the
    // object __vftable points at, named so HALO_SOUND_SYSTEM::~HALO_SOUND_SYSTEM can take its
    // address for the destructor vptr unwind, per the convention used across src/headers/hcex.
    // Merged in from the snd_fmod_boundary.h copy this pass deleted — it was the only one of the
    // two that carried it, and dropping it broke src/hcex/sound/HALO_SOUND_SYSTEM__dtor.cpp:14.
    static SYSTEM_CUSTOM_vtbl vftable;

    // Convenience declarations for the virtual interface above (dispatch via __vftable).
    bool Init(INIT initParams);
    void Term();
    void Update();
    void InitLevel(const dsTSTRING<char> &levelFullName);
    void TermLevel();
    void DbgRenderSounds();
} SYSTEM_CUSTOM;

// ?SystemCustom@snd@@3PAVSYSTEM_CUSTOM@1@A @ 0x84274EF8 -- process-wide custom sound system
// pointer (dynamically a HALO_SOUND_SYSTEM in this build).
extern SYSTEM_CUSTOM *SystemCustom;

} // namespace snd
