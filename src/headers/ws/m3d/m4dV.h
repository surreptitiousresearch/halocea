#pragma once
#include "m3dV.h"
// ws-engine m3d math: homogeneous 4-component vector.
// DB-verified layout (types_members m4dV): anonymous m3dV base @0 (x,y,z), w@12 — size 16.
// The DB models the leading m3dV as an anonymous member; reconstructed here as public
// inheritance so x/y/z are promoted alongside w.

struct m4dV : m3dV {
    float w;
};
