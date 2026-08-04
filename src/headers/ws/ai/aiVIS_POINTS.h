#pragma once
#include "../m3d/m3dV.h"
// ws-engine ai08: the 4 sampled body sight-points (world + LCS) for an enemy visibility probe.
// DB-verified layout (headers_ref aiVIS_POINTS) — size 0x60.

struct aiVIS_POINTS {
    m3dV posBasic;     // 0x00
    m3dV posBottom;    // 0x0C
    m3dV posRandom;    // 0x18
    m3dV posTop;       // 0x24
    m3dV posBasicLCS;  // 0x30
    m3dV posBottomLCS; // 0x3C
    m3dV posRandomLCS; // 0x48
    m3dV posTopLCS;    // 0x54

    // 0x83290C68 (?Clear@aiVIS_POINTS@@QAAXXZ) — reset world points to undefined, LCS points to zero.
    // REVERSED: src/ws/ai/aiVIS_POINTS__Clear.cpp
    void Clear();

    // Copy-assign (memberwise). DB mangle ?4aiVIS_POINTS@@QAAAAU0@ABU0@@Z. boundary — body external.
    aiVIS_POINTS &operator=(const aiVIS_POINTS &o);
};
