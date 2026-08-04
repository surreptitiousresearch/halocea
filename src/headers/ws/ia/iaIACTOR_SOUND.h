#pragma once
#include "../ds/LIST.h"
// ws-engine ia: per-actor sound state embedded (by value) in iaIACTOR::snd.
// DB-verified layouts (types_members iaIACTOR_SOUND / iaSND_PLAY_LIST / iaSND_AI_DATA
// and types_enum_values iaSND_AI_VALUE).

struct iaIACTOR;        // boundary — owning interactive actor
struct iaSND_PLAY_DATA; // boundary — one playing-sound record (list element)

// DB-verified (types_enum_values iaSND_AI_VALUE).
enum iaSND_AI_VALUE {
    SND_AI_VAL_NA   = -1,
    SND_AI_VAL_LOW  = 0,
    SND_AI_VAL_MED  = 1,
    SND_AI_VAL_HIGH = 2,
    SND_AI_VAL_LAST = 3,
};

// DB-verified layout (types_members iaSND_PLAY_LIST) — size 28.
typedef struct iaSND_PLAY_LIST {
    int                        numLoopSounds; // 0x00
    iaIACTOR                  *pOwner;        // 0x04
    ds::LIST<iaSND_PLAY_DATA>  list;          // 0x08 (20B)

    // Pause / resume every playing sound in the list. Pause takes a dsFLAGS<PAUSE_TYPE,int>
    // pause-cause word (entENTITY::implHide passes cause 1). boundary.
    void Pause(int pauseCause);
    void Resume();
} iaSND_PLAY_LIST;

// DB-verified layout (types_members iaSND_AI_DATA) — size 8.
typedef struct iaSND_AI_DATA {
    iaSND_AI_VALUE importance; // 0x00
    iaSND_AI_VALUE volume;     // 0x04
} iaSND_AI_DATA;

// DB-verified layout (types_members iaIACTOR_SOUND) — size 36.
typedef struct iaIACTOR_SOUND {
    iaSND_PLAY_LIST playList; // 0x00 (28B)
    iaSND_AI_DATA   aiNotify; // 0x1C (8B)
} iaIACTOR_SOUND;
