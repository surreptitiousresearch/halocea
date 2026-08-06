#pragma once
// Misc externs HALO_SOUND_LIST's and HALO_CHANNEL's methods touch that don't belong to any
// single sibling type: os-layer helpers, m3d math, the HCEX float3<->m3dV bridge, dbg-console
// tunables, and the ds string-building helpers this cluster's .cpp files use. None of these are
// reversed here (boundary / other-subsystem globals); declared only far enough to type call
// sites, matching the sibling HALO_SOUND_SYSTEM_globals.h convention.

#include "../ws/ds/dsTSTRING.h"
#include "../ws/m3d/m3dV.h"
#include "hcex_float3.h"

// ---- m3d math helpers (ws-engine m3d module; boundary) ----
extern "C" {
    bool  m3dCheckFinite(const m3dV *v);              // boundary
    void  _m3dCheckValid(double value);               // boundary -- range/NaN debug check
    float m3dLengthVector(const m3dV *v);              // boundary
    float m3dLengthVector_2(const m3dV *v);            // boundary -- squared-length variant
    float sqrtf(float x);                               // boundary -- CRT/libm (disasm: fsqrts)

    // HCEX float3 <-> m3dV conversion (HCEX bridge; boundary).
    void hcex_conv_pos(const hcex_float3 *src, m3dV *dst); // boundary
    void hcex_conv_vec(const hcex_float3 *src, m3dV *dst); // boundary

    // Notifies the outer ws-engine a Blam sound started playing (lip-sync/subtitle triggers). boundary.
    void hcex_notify_sound_start(const char *tagName);
}

// A tunable dbg-console value with change-detection, matching HALO_SOUND_SYSTEM_globals.h's
// DBG_VAR<T> convention (redeclared here to avoid a cross-header dependency loop).
template<class T>
struct DBG_VAR {
    T value;
    T prevValue;
};

extern "C" DBG_VAR<bool> dbg_printBufPlay_10;  // HALO_SOUND_LIST::GetFreeSound / HALO_CHANNEL::CheckNotReady debug log
extern "C" DBG_VAR<bool> dbg_printQueuePlay;   // HALO_CHANNEL::Queue debug log
extern "C" DBG_VAR<bool> dbg_disableEAX;       // HALO_CHANNEL::CheckNotReady -- force master (non-reverb) bus
extern "C" DBG_VAR<bool> dbg_disable3DCone;    // HALO_CHANNEL::UpdateProperties -- disable 3D cone attenuation
extern "C" DBG_VAR<bool> dbg_mute2dSounds;     // HALO_CHANNEL::UpdateProperties -- mute all 2D sounds

// ---- ds string helpers (template dsTSTRING<char> convention) ----
extern const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary
// Strips the name+extension off a path (result written to *out). boundary.
extern dsTSTRING<char> *fnmGetNameExtStr(dsTSTRING<char> *out, const dsTSTRING<char> *path);

// dsTSTRING<char> concatenation (sret out-param first) -- two overloads observed at
// HALO_CHANNEL::CheckNotReady's debug "PLAY ..." log line: literal + dsTSTRING, and
// dsTSTRING + literal. In the binary these are operator+ overloads, but C++ forbids a binary
// operator+ with an explicit sret out-param (3 params); expose them as named free functions
// (call sites use the same explicit-call form). boundary.
template<class T>
const dsTSTRING<T> *dsTSTRING_concat(dsTSTRING<T> *out, const char *lhs, const dsTSTRING<T> *rhs); // boundary
template<class T>
const dsTSTRING<T> *dsTSTRING_concat(dsTSTRING<T> *out, const dsTSTRING<T> *lhs, const char *rhs); // boundary
