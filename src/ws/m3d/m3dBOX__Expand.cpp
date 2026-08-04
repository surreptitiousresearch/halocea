#include "m3dBOX.h"

// m3dBOX::Expand @ 0x8265BEB8
// Inflate the box by `value` on every axis (min corner out, max corner out).
// DEVIATION: the decompiler typed the parameter `double`; the mangled name (?...@@QAAXM@Z,
// M = float) and the `lfs`/single-precision arithmetic show it is `float`.
void m3dBOX::Expand(float value)
{
    bll.x -= value;
    bll.y -= value;
    bll.z -= value;
    fur.x += value;
    fur.y += value;
    fur.z += value;
}
