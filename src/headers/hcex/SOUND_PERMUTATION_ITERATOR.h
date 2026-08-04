#pragma once
// Legacy Blam sound-tag traversal helper: walks sound_definition -> sound_pitch_range ->
// sound_permutation to build the cache-relative path string used as the FSB lookup key
// (HALO_SOUND_SYSTEM::FindSoundByName). Blam-side type; full body belongs to the sound-tag
// subsystem, not the HCEX bridge — declared here only far enough to type the two call sites
// HALO_SOUND_SYSTEM uses (construct-from-permutation, BuildPath).
// DB-verified layout (types_members SOUND_PERMUTATION_ITERATOR): sound@0
// (sound_definition*), rangeIndex@4, range@8 (sound_pitch_range*), permIndex@12,
// perm@16 (sound_permutation*) — size 20.

struct sound_definition;   // boundary — Blam tag runtime object
struct sound_pitch_range;  // boundary — Blam tag runtime object
struct sound_permutation;  // full layout in ../sound_permutation.h

typedef struct SOUND_PERMUTATION_ITERATOR {
    sound_definition   *sound;       // 0x00
    int                  rangeIndex;  // 0x04
    sound_pitch_range   *range;        // 0x08
    int                  permIndex;     // 0x0C
    sound_permutation   *perm;           // 0x10

    // boundary — Blam sound-tag subsystem; external to this batch.
    SOUND_PERMUTATION_ITERATOR(const sound_permutation *perm);
    void BuildPath(char *path, int length, bool withPerm);
} SOUND_PERMUTATION_ITERATOR;
