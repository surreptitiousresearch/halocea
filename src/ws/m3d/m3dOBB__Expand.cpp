#include "m3dOBB.h"
#include "m3d_boundary.h"

// m3dOBB::Expand @ 0x82A6F0C0
// Inflate the oriented box by `value` on every axis: push the origin back by `value` along
// each unit axis (vx, vy, vz) and grow each half-extent by 2*value.
//
// NOTE: the shipped STRONG_ASSERT(sN >= 0.f) macros are reproduced as their expansion — a
// guard on the global IGNORE_STRONG_ASSERT plus a Crash call invoked on a null dummy `this`
// (STRONG_ASSERT_DUMMY), carrying the original expression, source path and line.
// DEVIATION: decompiler typed `value` as double; mangle ?Expand@m3dOBB@@QAAXM@Z (M) and the
// single-precision arithmetic show it is float.
void m3dOBB::Expand(float value)
{
    float back = -value;

    _m3dCheckValid(&org);
    _m3dCheckValid(&vx);
    _m3dCheckValid(back);
    org.x = vx.x * back + org.x;
    org.y = vx.y * back + org.y;
    org.z = vx.z * back + org.z;

    _m3dCheckValid(&org);
    _m3dCheckValid(&vy);
    _m3dCheckValid(back);
    org.x = vy.x * back + org.x;
    org.y = vy.y * back + org.y;
    org.z = vy.z * back + org.z;

    _m3dCheckValid(&org);
    _m3dCheckValid(&vz);
    _m3dCheckValid(back);
    org.x = vz.x * back + org.x;
    org.y = vz.y * back + org.y;
    org.z = vz.z * back + org.z;

    float grow = value * 2.0f;

    sx = sx + grow;
    if (!IGNORE_STRONG_ASSERT && sx < 0.0f)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "sx >= 0.f", "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_obb.cpp", 767,
            m3dStrongAssertMessage);

    sy = sy + grow;
    if (!IGNORE_STRONG_ASSERT && sy < 0.0f)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "sy >= 0.f", "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_obb.cpp", 769,
            m3dStrongAssertMessage);

    sz = sz + grow;
    if (!IGNORE_STRONG_ASSERT && sz < 0.0f)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "sz >= 0.f", "D:\\Projects\\code\\common\\src.sys\\m3d\\m3d_obb.cpp", 771,
            m3dStrongAssertMessage);
}
