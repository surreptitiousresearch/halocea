#pragma once

/* GPU_TESSELLATORPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_TESSELLATORPACKET). */
#include "GPU_ENHANCE.h"
#include "GPU_GROUPPRIMTYPE.h"
#include "GPU_GROUPVECTCONTROL.h"
#include "GPU_GROUPVECTFMTCONTROL.h"
#include "GPU_HOSCONTROL.h"
#include "GPU_LINECONTROL.h"
#include "GPU_MPASSPSCONTROL.h"
#include "GPU_OUTPUTPATHCONTROL.h"
#include "GPU_POINTMINMAX.h"
#include "GPU_POINTSIZE.h"
#include "GPU_VIZQUERY.h"

typedef struct GPU_TESSELLATORPACKET { // DB types_members GPU_TESSELLATORPACKET
    GPU_POINTSIZE PointSize; /* 0x00 */
    GPU_POINTMINMAX PointMinMax; /* 0x04 */
    GPU_LINECONTROL LineControl; /* 0x08 */
    unsigned int Unused1; /* 0x0C */
    GPU_OUTPUTPATHCONTROL OutputPathControl; /* 0x10 */
    GPU_HOSCONTROL HosControl; /* 0x14 */
    float HosMaxTessLevel; /* 0x18 */
    float HosMinTessLevel; /* 0x1C */
    unsigned int HosReuseDepth; /* 0x20 */
    GPU_GROUPPRIMTYPE GroupPrimType; /* 0x24 */
    unsigned int GroupFirstDecr; /* 0x28 */
    unsigned int GroupDecr; /* 0x2C */
    GPU_GROUPVECTCONTROL GroupVect0Control; /* 0x30 */
    GPU_GROUPVECTCONTROL GroupVect1Control; /* 0x34 */
    GPU_GROUPVECTFMTCONTROL GroupVect0FmtControl; /* 0x38 */
    GPU_GROUPVECTFMTCONTROL GroupVect1FmtControl; /* 0x3C */
    unsigned int Unused2[2]; /* 0x40 */
    GPU_MPASSPSCONTROL MPassPsControl; /* 0x48 */
    GPU_VIZQUERY VizQuery; /* 0x4C */
    GPU_ENHANCE Enhance; /* 0x50 */
} GPU_TESSELLATORPACKET;
