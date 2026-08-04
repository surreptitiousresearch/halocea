#pragma once
// Boundary externs for the HALO_CHANNEL method cluster (src/hcex/HALO_CHANNEL__*.cpp).
// Pulls in the owning type (HALO_SOUND_SYSTEM brings HALO_CHANNEL/HALO_PERM_SOUND/
// HALO_SOUND_LIST/TAG_INFO/HALO_CHANNEL_PROPERTIES/HALO_SOUND_PARAMS + the snd::/FMOD boundary),
// the shared misc-globals header (m3d helpers, dbg vars, dsSPrintf, dsTSTRING_concat, hcex_conv),
// and the ws-engine dbg/os/mem boundaries the bodies touch. None of these are reversed here.

#include "HALO_SOUND_SYSTEM.h"
#include "HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../ws/dbg/STRONG_ASSERT2_HELPER.h"
#include "../ws/ds/dsTSTRING.h"

// Blam boundary type (../sound_location.h). Only the leading position/forward/
// translational_velocity real-vector triplet is used here; each is a 3-float vector, so the whole
// prefix is layout-compatible with hcex_float3[3] (which is how SetLocation consumes it).
// DB-verified offsets (types_members sound_location); game_location@36 is the Blam `location`
// (cluster+leaf), typed via ../location.h (byteshim reconciliation 2026-08-04).
#include "../location.h"
struct sound_location {
    hcex_float3 position;               // 0x00 (real_point3d)
    hcex_float3 forward;                // 0x0C (real_vector3d)
    hcex_float3 translational_velocity; // 0x18 (real_vector3d)
    location    game_location;          // 0x24 (Blam `location`, 8 bytes)
};

// Blam boundary type (../platform_sound_channel_properties.h). DB-verified layout
// (types_members platform_sound_channel_properties) — size 32.
struct platform_sound_channel_properties {
    float minimum_distance;    // 0x00
    float maximum_distance;    // 0x04
    float pitch;               // 0x08
    float gain;                // 0x0C
    float inner_cone_angle;    // 0x10
    float outer_cone_angle;    // 0x14
    float outer_cone_gain;     // 0x18
    float reverb_damping_factor; // 0x1C
};

// ---- ws-engine dbg / os / mem boundary globals & helpers ----
extern int IGNORE_STRONG_ASSERT;    // ?IGNORE_STRONG_ASSERT@@3HA
extern const char empty_string;    // shared single-NUL empty-string constant

#include "../ws/dbg/STRONG_ASSERT_DUMMY.h" // canonical STRONG_ASSERT_DUMMY::Crash (avoids C2011)

extern "C" {
    void  dlFree(void *p);          // ws-engine allocator free
    void  osDebugBreak();           // os-layer debugger break
    unsigned int osGetCurThreadId(); // src/ws/os/osGetCurThreadId.cpp
    void  osOutputDebugString(const char *fmt, ...); // printf-style debug output
    char *tag_get_name(short tag_index); // Blam tag-name lookup (src/tag_get_name.c)
}
