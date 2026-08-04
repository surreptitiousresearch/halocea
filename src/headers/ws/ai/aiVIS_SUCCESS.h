#pragma once
#include "aiVIS_POINTS.h"
#include "CONST_ARRAY_m3dV.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: last-successful enemy-visibility sample (the sight points + free-space points).
// DB-verified layout (headers_ref aiVIS_SUCCESS) — size 0xE8.

struct aiVIS_SUCCESS {
    aiVIS_POINTS         points;         // 0x00
    CONST_ARRAY_m3dV<10> pointsFree;     // 0x60
    m3dV                 posFreeAverage; // 0xDC
};
