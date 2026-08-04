#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine snd: an abstract, project-supplied sound system override (distinct root hierarchy
// from snd::SYSTEM -- its own vtable, not derived from SYSTEM). HCEX's global `SystemCustom`
// points at a HALO_SOUND_SYSTEM instance at runtime. BOUNDARY -- only the vtable shape touched by
// HALO_MNG::ProcessMsg/ProcessTERM_LEVEL is modeled; not decompiled as a concrete class here.
// DB-verified layout (types_members snd::SYSTEM_CUSTOM_vtbl): Init@0, Term@4, Update@8,
// InitLevel@12, TermLevel@16, DbgRenderSounds@20, destructor@24.
// DB-verified (types_members snd::SYSTEM_CUSTOM): __vftable@0 -- size 4.

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
    void (*dtr_SYSTEM_CUSTOM)(SYSTEM_CUSTOM *self);                        // 0x18
} SYSTEM_CUSTOM_vtbl;

typedef struct SYSTEM_CUSTOM {
    SYSTEM_CUSTOM_vtbl *__vftable; // 0x00

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
