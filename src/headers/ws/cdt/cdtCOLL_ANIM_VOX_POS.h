#pragma once
#include "../m3d/m3dVINT.h"
// ws-engine cdt: per-instance cached voxel-cell span for the collision-animation scenes.
// DB-verified layout (types_members cdtCOLL_ANIM_VOX_POS, size 24): from@0, to@12 (both m3dVINT).

typedef struct cdtCOLL_ANIM_VOX_POS {
    m3dVINT from; // 0x00 first occupied voxel cell
    m3dVINT to;   // 0x0C last occupied voxel cell
} cdtCOLL_ANIM_VOX_POS;
