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

// Blam boundary type: the canonical definition, matching types_members sound_location
// (real_point3d position@0x00, real_vector3d forward@0x0C, real_vector3d
// translational_velocity@0x18, location game_location@0x24 — size 44). This header used to carry
// its own copy of the struct with the three vectors spelled hcex_float3; layout-identical, but a
// second definition of a Blam type that already owns a header, and it disagreed with the DB's
// member types. Pull the canonical one in instead.
#include "../sound_location.h"

// Blam boundary type. DB-verified layout (types_members platform_sound_channel_properties) — size
// 32. Same treatment as sound_location above: this header used to carry its own file-scope copy of
// the eight-float body, member-for-member identical to the type's own header, so the layout probe
// measured it twice. Pull the canonical one in instead.
#include "../platform_sound_channel_properties.h"

// ---- ws-engine dbg / os / mem boundary globals & helpers ----
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

#include "../ws/dbg/STRONG_ASSERT_DUMMY.h" // canonical STRONG_ASSERT_DUMMY::Crash (avoids C2011)

extern "C" {
    void  dlFree(void *p);          // ws-engine allocator free
    void  osDebugBreak();           // os-layer debugger break
    unsigned int osGetCurThreadId(); // src/ws/os/osGetCurThreadId.cpp
    void  osOutputDebugString(const char *fmt, ...); // printf-style debug output
    char *tag_get_name(short tag_index); // Blam tag-name lookup (src/tag_get_name.c)
}
