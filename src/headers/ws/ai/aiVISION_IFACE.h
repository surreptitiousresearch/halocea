#pragma once
#include "aiVISION_BASE.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: aiVISION_IFACE — an aiVISION_BASE bound to a specific brain. DB/headers_ref
// aiVISION_IFACE (size 0x8): aiVISION_BASE base @0x00, pBrain @0x04. Base of aiVISION_COMMON and
// aiVISION_ZERO.

struct aiBRAIN; // ai08 — per-unit AI brain (full def aiBRAIN.h)   boundary (ptr)

struct aiVISION_IFACE : aiVISION_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain

    // 0x832910C0 (?CalcPosEye@aiVISION_IFACE@@UAA?AUm3dV@@XZ) — virtual, sret: the brain's eye
    // sample position (the status face position, adjusted by the best-behaviour module).
    // REVERSED: src/ws/ai/aiVISION_IFACE__CalcPosEye.cpp
    m3dV *CalcPosEye(m3dV *out);
};
