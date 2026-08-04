#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine ent: the sound-effect list owned by an entENTITY (entENTITY::sfxList).
// DB-verified layout (types_members entSFX_LIST) — size 28.

struct entSFX; // ent subsystem — one active sound-effect instance (pointer only)   boundary

typedef struct entSFX_LIST {
    dsVECTOR<entSFX *, 8> sfxArray;        // 0x00 live sfx instances
    unsigned int          sfxColor;        // 0x14
    float                 timeScaleOnInit; // 0x18

    // Sfx-list operations driven by entENTITY show/hide. Length/Get expose the live instances;
    // Pause/Resume and Hide/Show broadcast the corresponding transition to each. boundary.
    int     Length() const;
    entSFX *Get(int i) const;
    void    Pause();
    void    Resume();
    void    Hide();
    void    Show();
} entSFX_LIST;
