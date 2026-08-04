#pragma once
#include "../m3d/m3dV.h"
#include "../m3d/m3d_boundary.h"   // m3dVZero, _m3dCheckValid, m3dLengthVector, IGNORE_STRONG_ASSERT, STRONG_ASSERT_DUMMY
#include "../wb/dbgVAR_boundary.h" // dbgVAR_IMPL<bool,1>
// Free functions, math operators, and global con-vars referenced by the reversed aiWEAPON strike
// methods but whose bodies/definitions live outside this batch. Declared here as boundaries so the
// re-sourced .cpp translation units are self-contained.

// Squared-length tolerance below which a cached muzzle tip is treated as "not yet computed"
// (float literal pool constant the decompiler names M3D_EPSILON_2_623). boundary.
extern const float M3D_EPSILON_2_623;

// --- m3d free operators (by-value; ABI is sret) -------------------------------------------------
m3dV operator-(const m3dV &a, const m3dV &b);
m3dV operator*(const m3dV &v, float s);
m3dV operator*(float s, const m3dV &v);

// NaN-filled sentinel "undefined position" vector (?m3dVUndef@@3Um3dV@@A).
extern const m3dV m3dVUndef;

// Tolerant point equality (?m3dIsEqualPoint_A@@YA_NPBUm3dV@@0M@Z per-component |a-b| <= eps).
bool m3dIsEqualPoint_A(const m3dV *a, const m3dV *b, float eps);

// Count `*field` down by `dt`, clamping at 0; returns whether it reached 0 this call
// (?m3dUpdateTimeField@@YAHPAMM@Z).
int m3dUpdateTimeField(float *field, float dt);

// Component of `v` tangent to (perpendicular to) unit axis `norm`, written to `out`
// (?m3dTangentComponent@@YAXPIBUm3dV@@0PIAU1@@Z). out may alias v.
void m3dTangentComponent(const m3dV *v, const m3dV *norm, m3dV *out);

// ai08 debug con-var: when set, weapon tip positions are overridden from the debug camera.
extern dbgVAR_IMPL<bool, 1> dbg_defCDT_test;

// Debug helper: current position derived from the debug camera (sret). boundary.
m3dV *aiDbgGetPosFromCamera(m3dV *out);

// Shared single-NUL-byte empty string constant, passed as the STRONG_ASSERT context arg. The
// reversed bodies reference the IDA raw name byte_8200155A; declared here matching the
// Crash(...,unsigned char msgCtx) overload it resolves to. (2026-07-31)
extern const char empty_string;
extern unsigned char byte_8200155A;

// ai08 tuning constant (?WPN_TIME_KILL_HANG@@3MB / .data float @ ~0x820D9A10) — the "kill-hang"
// grace window (seconds) a weapon module holds after starting to perform a strike. Value 6.5.
extern const float WPN_TIME_KILL_HANG;
