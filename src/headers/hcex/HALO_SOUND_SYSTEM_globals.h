#pragma once
// Misc externs HALO_SOUND_SYSTEM's methods touch that don't belong to any single sibling type:
// os-layer helpers, the Blam tag-name lookup, dbg-console tunables, and the fixed FSB
// sound-list table. None of these are reversed here (boundary / other-subsystem globals);
// declared only far enough to type HALO_SOUND_SYSTEM's call sites.

#include "../ws/ds/dsTSTRING.h"

// ---- os-layer (real bodies already exist in src/ws/os/, declared via os_boundary.h normally;
// redeclared here narrowly since those .cpp files don't expose a shared prototype header) ----
extern "C" {
    unsigned int osGetCurThreadId();                  // src/ws/os/osGetCurThreadId.cpp
    unsigned int osGetTime();                          // src/ws/os/osGetTime.cpp
    void         osOutputDebugString(const char *fmt, ...); // boundary — printf-style wrapper over _osOutputDebugString
    char        *tag_get_name(short tag_index);         // src/tag_get_name.c
}

// A tunable dbg-console value with change-detection (Update() compares .value against
// .prevValue to notice a live edit). boundary — dbg-console variable subsystem.
template<class T>
struct DBG_VAR {
    T value;
    T prevValue;
};

extern DBG_VAR<bool> dbg_disableSounds_0;     // globally mutes HALO_SOUND_SYSTEM::FindSoundByName
extern DBG_VAR<int>  dbg_mouthMinDelay;        // -1 == "use HALO_SOUND_PARAMS::mouthMinDelay"
extern DBG_VAR<bool> dbg_pauseHaloSounds;      // dbg-console pause toggle, polled from Update()
extern int            maxPlaying;               // high-water mark logged by PrintUsedSounds
extern bool            hcex_mouth_skull;         // HCEX lip-sync/skull-VO feature toggle

// One entry of the fixed 15-element FSB sound-list table (types_members
// _EC05441F58351F3B07D4C7AF18902550): prepareStreams@0, name@4, filename@8, level@12 — size 16.
// Anonymous aggregate-literal type in the original source; DB $-name kept as the tag
// ($ in identifiers is an MSVC extension), aliased to a readable name.
typedef struct _EC05441F58351F3B07D4C7AF18902550 {
    int          prepareStreams; // 0x00
    const char  *name;            // 0x04
    const char  *filename;         // 0x08
    const char  *level;             // 0x0C
} _EC05441F58351F3B07D4C7AF18902550;
typedef _EC05441F58351F3B07D4C7AF18902550 SOUND_LIST_ENTRY;
extern SOUND_LIST_ENTRY SOUND_LIST_LST[15]; // fixed table, one entry per HALO_SOUND_SYSTEM::soundLists slot

#include "../sound_listener.h"
struct platform_sound_manager_definition; // Blam platform sound layer — opaque here

// Blam sound-manager config globals (`sound_manager_globals_t sound_manager_globals;`) —
// full DB layout (types_members sound_manager_globals_t, size 188).
typedef struct sound_manager_globals_t {
    unsigned char initialized;                                  // 0x00
    unsigned char active;                                       // 0x01
    unsigned char paused;                                       // 0x02
    unsigned char idling;                                       // 0x03
    int game_time_when_no_scripted_dialog_will_be_playing;      // 0x04
    platform_sound_manager_definition *platform;                // 0x08
    int render_time;                                            // 0x0C
    float ticks_elapsed;                                        // 0x10
    unsigned char flip_flop;                                    // 0x14
    unsigned char _pad15[3];                                    // 0x15
    sound_listener listeners[2];                                // 0x18
    int sound_environment_tag;                                  // 0xA0
    float nondialog_gain;                                       // 0xA4
    float music_gain;                                           // 0xA8
    float master_gain;                                          // 0xAC
    float effects_gain;                                         // 0xB0
    __int16 channel_count;                                      // 0xB4
    unsigned char disable_from_commandline;                     // 0xB6
    unsigned char _padB7;                                       // 0xB7
    __int16 sound_variety;                                      // 0xB8
} sound_manager_globals_t;
extern sound_manager_globals_t sound_manager_globals;
