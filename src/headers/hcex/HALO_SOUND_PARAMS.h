#pragma once
// HCEX sound bridge: per-tag-name playback tuning parameters loaded from
// "d:\sounds\xbox360\tags_params.ps" (see HALO_SOUND_SYSTEM::LoadTagParams).
// DB-verified layout (types_members HALO_SOUND_PARAMS): tag@0 (dsSTRID), state@4
// (dsFLAGS<HALO_SOUND_PARAMS_STATE,int>), volume@8, volume2D@12, distGt@16, distGtTag@20
// (dsSTRID), distLt@24, distLtTag@28 (dsSTRID), dist2D@32, dist3D@36, mouthTag@40 (dsSTRID),
// mouthMinDelay@44, replace@48 (dsVECTOR<REPLACE_INFO,8>) — size 68.
// Sibling type: owned by the parallel HALO_SOUND_PARAMS drain; declared here from the DB only
// far enough to type HALO_SOUND_SYSTEM's field reads.

#include "../ws/ds/dsSTRID.h"
#include "../ws/ds/dsFLAGS.h"
#include "../ws/ds/dsVECTOR.h"

// types_enum_values HALO_SOUND_PARAMS_STATE.
enum HALO_SOUND_PARAMS_STATE {
    HALO_SOUND_HAS_DIST_LT = 1,
    HALO_SOUND_HAS_DIST_GT = 2,
    HALO_SOUND_UPDATE_PANLEVEL = 4,
    HALO_SOUND_PLAY_TO_END = 8,
    HALO_SOUND_HAS_MOUTH_SOUND = 16,
};

typedef struct HALO_SOUND_PARAMS {
    // CMP -- stateless ascending-by-tag comparator used by dsVECTOR<HALO_SOUND_PARAMS,8>
    // ::FindSorted / ds::Sort over haloSoundSystem->params / ->paramsWildcard. Both overloads
    // do a plain byte-wise strcmp over dsSTRID::CStr(); no instance state (stateless functor).
    // DEVIATION: decompiler shows pointer params throughout (its convenience prototype string
    // always renders references as pointers); both mangles decode `a`/`b` as `ABU2@@` =
    // reference-to-const, so both overloads take `const HALO_SOUND_PARAMS &`.
    struct CMP {
        static int cmp(const HALO_SOUND_PARAMS &a, const HALO_SOUND_PARAMS &b); // ?cmp@CMP@HALO_SOUND_PARAMS@@SAHABU2@0@Z
        static int cmp(const HALO_SOUND_PARAMS &a, const char *name);          // ?cmp@CMP@HALO_SOUND_PARAMS@@SAHABU2@PBD@Z
    };

    // DB-verified layout (types_members HALO_SOUND_PARAMS::REPLACE_INFO): source@0 (dsSTRID),
    // tag@4 (dsSTRID) — size 8.
    struct REPLACE_INFO {
        dsSTRID source; // 0x00 tag-name to match against an incoming identifier
        dsSTRID tag;    // 0x04 replacement tag-name to play instead

        REPLACE_INFO(); // ??0REPLACE_INFO@HALO_SOUND_PARAMS@@QAA@XZ — default-constructs both dsSTRID members
    };

    dsSTRID                                  tag;           // 0x00
    dsFLAGS<HALO_SOUND_PARAMS_STATE, int>    state;         // 0x04
    float                                     volume;        // 0x08
    float                                     volume2D;      // 0x0C
    float                                     distGt;        // 0x10
    dsSTRID                                   distGtTag;     // 0x14
    float                                     distLt;        // 0x18
    dsSTRID                                   distLtTag;     // 0x1C
    float                                     dist2D;        // 0x20
    float                                     dist3D;        // 0x24
    dsSTRID                                   mouthTag;      // 0x28
    int                                        mouthMinDelay; // 0x2C
    dsVECTOR<REPLACE_INFO, 8>                 replace;       // 0x30

    HALO_SOUND_PARAMS();                             // ??0HALO_SOUND_PARAMS@@QAA@XZ (default)
    HALO_SOUND_PARAMS(const HALO_SOUND_PARAMS &that); // ??0HALO_SOUND_PARAMS@@QAA@ABU0@@Z (copy)
    ~HALO_SOUND_PARAMS();                             // ??1HALO_SOUND_PARAMS@@QAA@XZ
    HALO_SOUND_PARAMS &operator=(const HALO_SOUND_PARAMS &that); // ??4HALO_SOUND_PARAMS@@QAAAAU0@ABU0@@Z
    HALO_SOUND_PARAMS *scalarDeletingDtor(unsigned char deleteFlags); // ??_GHALO_SOUND_PARAMS@@QAAPAXI@Z

    // Parses this entry's fields out of a script section (see .cpp for the exact key names:
    // "volume", "volume2D", "onDistFar.name"/".value", "onDistNear.name"/".value",
    // "panLevel.dist2D"/".dist3D", "mouthName", "mouthMinDelay", "isPlayToEnd", "replace").
    void Init(const struct psSECTION &section); // ?Init@HALO_SOUND_PARAMS@@QAAXABVpsSECTION@@@Z
} HALO_SOUND_PARAMS;
