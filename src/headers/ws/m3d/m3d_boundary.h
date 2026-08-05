#pragma once
#include "m3dV.h"
// External m3d-subsystem free functions and global constants referenced by the reversed
// m3d methods but not themselves reversed in this batch — declared here as boundaries.

struct m3dMATR;

// Unit basis vectors (global constants).
extern const m3dV m3dVUnitX;
extern const m3dV m3dVUnitY;
extern const m3dV m3dVUnitZ;
// Zero vector (global constant).
extern const m3dV m3dVZero;
// Identity 4x3 transform (global constant, ?m3dMatrIdentity@@3Vm3dMATR@@A).
extern m3dMATR m3dMatrIdentity;

// out = scalar * v (?D@YA?AUm3dV@@MABU0@@Z) — scalar-vector product, result by value.  boundary.
m3dV operator*(float scalar, const m3dV &v);
// out = v * scalar (??D@YA?AUm3dV@@ABU0@M@Z @ 0x82561EF0) — vector-scalar product, by value. boundary.
m3dV operator*(const m3dV &v, float scalar);
// out = a - b (??G@YA?AUm3dV@@ABU0@0@Z @ 0x825E0D48) — component-wise subtract, by value. boundary.
m3dV operator-(const m3dV &a, const m3dV &b);
// out = a + b (??H@YA?AUm3dV@@ABU0@0@Z @ 0x8274B3F0) — component-wise add, by value. boundary.
m3dV operator+(const m3dV &a, const m3dV &b);

// Debug finiteness assertions; overloaded for a scalar and for a vector.
// DB return type is int (?_m3dCheckValid@@YAHM@Z / ...@YAHPBUm3dV@@@Z); callers that
// only want the side effect ignore it.
// REVERSED: src/ws/m3d/_m3dCheckValid.cpp (float overload), _m3dCheckValid__2.cpp (vector
// overload). Declared here too since every other m3d .cpp already includes this header to
// get these prototypes.
int _m3dCheckValid(float value);
int _m3dCheckValid(const m3dV *v);

// Non-throwing finiteness predicate (?m3dCheckFinite@@YA_NM@Z).
// REVERSED: src/ws/m3d/m3dCheckFinite.cpp.
bool m3dCheckFinite(float value);

// Euclidean length of a vector.
// REVERSED: src/ws/m3d/m3dLengthVector.cpp.
float m3dLengthVector(const m3dV *v);

// Squared Euclidean length (?m3dLengthVector_2@@YAMPIBUm3dV@@@Z) — boundary.
float m3dLengthVector_2(const m3dV *v);

// Advance the countdown at *pField toward zero by `dt` seconds (clamped at 0); returns nonzero
// while still running (?m3dUpdateTimeField@@YAHPAMM@Z @ 0x82C3C2F8). boundary.
int m3dUpdateTimeField(float *pField, float dt);

// Clamp `value` into [rangeLo,rangeHi] then remap onto [outAtLo,outAtHi], flooring negative
// results at zero (?m3dClampZero@@YAMMMMMM@Z — float ret + 5 float params) — boundary.
float m3dClampZero(float outAtLo, float outAtHi, float rangeLo, float rangeHi, float value);

// In-place scale / negate / line-combine helpers (underscore = destructive variant) — boundary.
void _m3dScaleVector(m3dV *v, float scale);            // ?_m3dScaleVector@@YAXPIAUm3dV@@M@Z
void _m3dNegateVector(m3dV *v);                        // ?_m3dNegateVector@@YAXPIAUm3dV@@@Z
// out = base + dir * t (?_m3dLineCombine@@YAXPIBUm3dV@@0MPIAU1@@Z).
// REVERSED: src/ws/m3d/_m3dLineCombine.cpp.
void _m3dLineCombine(const m3dV *base, const m3dV *dir, float t, m3dV *out);

// Distance from `point` to the line (linePoint, lineDir) (?m3dDistPointLine@@YAMPBUm3dV@@00@Z).
float m3dDistPointLine(const m3dV *point, const m3dV *linePoint, const m3dV *lineDir);
// Overload (?m3dDistPointLine@@YAMPBUm3dV@@00PAU1@@Z) — same distance, and also write the closest
// point on the segment (segFrom, segTo) to `point` into `outClosest`.
float m3dDistPointLine(const m3dV *point, const m3dV *segFrom, const m3dV *segTo, m3dV *outClosest);

// Euclidean distance between two points (?m3dDist@@YAMPBUm3dV@@0@Z), and an alternate metric
// variant (?m3dDist_2@@...) — boundary.
float m3dDist(const m3dV *a, const m3dV *b);
float m3dDist_2(const m3dV *a, const m3dV *b);

// Normalise a vector in place (?m3dNormalize@@YAXPIAUm3dV@@@Z); the underscore variant
// returns whether the vector had non-zero length (?_m3dNormalize@@YAHPIAUm3dV@@@Z).
// REVERSED: src/ws/m3d/m3dNormalize.cpp. _m3dNormalize (bool-returning variant) remains
// an un-reversed boundary.
void m3dNormalize(m3dV *v);
int _m3dNormalize(m3dV *v);

// Component of `v` ALONG `axis`, i.e. the projection of v onto axis: (v . axis) * axis
// (?m3dNormalComponent@@YAXPIBUm3dV@@0PIAU1@@Z). CORRECTION: an earlier pass here guessed
// this was the orthogonal residual (v minus its projection); the disasm confirms it is the
// parallel projection itself, with no subtraction of v.
// REVERSED: src/ws/m3d/m3dNormalComponent.cpp.
void m3dNormalComponent(const m3dV *v, const m3dV *axis, m3dV *out);

// Angle between two vectors, in DEGREES (not radians -- disasm shows a *57.295776 scale
// after acos) (?m3dAngleVector@@YAMPBUm3dV@@0@Z), and its cosine-only sibling.
// REVERSED: src/ws/m3d/m3dAngleVector.cpp, src/ws/m3d/m3dAngleVector_COS.cpp.
float m3dAngleVector(const m3dV *a, const m3dV *b);
float m3dAngleVector_COS(const m3dV *a, const m3dV *b);

// out = a x b (right-handed cross product).
// REVERSED: src/ws/m3d/m3dCrossProduct.cpp.
void m3dCrossProduct(const m3dV *a, const m3dV *b, m3dV *out);

// Distance between two points in the XZ plane only (ignores Y). Used by m3dBOX::DistPoint for
// edge/corner cases. boundary — not reversed in this batch.
float m3xzDist(const m3dV *a, const m3dV *b);

// Clamp `value` into [rangeLo,rangeHi] (order-independent) then linearly remap onto
// [outAtLo,outAtHi] (?m3dClamp@@YAMMMMMM@Z -- 5 float params, not 6; verified via disasm
// prologue). REVERSED: src/ws/m3d/m3dClamp.cpp.
float m3dClamp(float outAtLo, float outAtHi, float rangeLo, float rangeHi, float value);

// Sign of a scalar: +1 for value >= 0, -1 otherwise (?m3dSign@@YAMM@Z) — boundary, used by
// m3dMATR::GetRotateRot's gimbal-lock branch.
float m3dSign(float value);

// Simple 3-arg clamp: clamp `value` into [min(a,b),max(a,b)] (?_m3dClamp@@YAMMMM@Z).
// Sibling of m3dClamp at a different address (0x823F2EE0); used by m3dAngleVector_COS but
// not itself in this batch's target list -- boundary only.
float _m3dClamp(float a, float b, float value);

// Deterministic position-based hash of a vector, scaled by `stride`
// (?m3dVRand@@YAKABUm3dV@@K@Z). REVERSED: src/ws/m3d/m3dVRand.cpp,
// src/ws/m3d/m3dVRandIntRange.cpp.
unsigned int m3dVRand(const m3dV &vv, unsigned int stride);
int m3dVRandIntRange(const m3dV &v, int lo, int hi, unsigned int stride);

// m3dMultiplyMatr is reversed (src/ws/m3d/m3dMultiplyMatr.cpp); declared in m3dMATR.h.

// --- Debug assertion machinery (Saber STRONG_ASSERT), boundary only ---------------
// Global that disables the strong-assert crash path (?IGNORE_STRONG_ASSERT@@3HA).
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
// Empty context/message string global at 0x8200155A, passed as the final Crash argument.
extern const char m3dStrongAssertMessage[];

// STRONG_ASSERT_DUMMY / STRONG_ASSERT2_HELPER now live in their canonical dbg headers so co-inclusion
// with another *_boundary.h that also needs them does not trigger C2011 redefinition.
#include "../dbg/STRONG_ASSERT_DUMMY.h"
#include "../dbg/STRONG_ASSERT2_HELPER.h"
