#pragma once

/* GPU_CONTROLPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_CONTROLPACKET). */
#include "GPU_BLENDCONTROL.h"
#include "GPU_CLIPCONTROL.h"
#include "GPU_COLORCONTROL.h"
#include "GPU_DEPTHCONTROL.h"
#include "GPU_EDRAMMODECONTROL.h"
#include "GPU_HICONTROL.h"
#include "GPU_MODECONTROL.h"
#include "GPU_VTECONTROL.h"

typedef struct GPU_CONTROLPACKET { // DB types_members GPU_CONTROLPACKET
    GPU_DEPTHCONTROL DepthControl; /* 0x00 */
    GPU_BLENDCONTROL BlendControl0; /* 0x04 */
    GPU_COLORCONTROL ColorControl; /* 0x08 */
    GPU_HICONTROL HiControl; /* 0x0C */
    GPU_CLIPCONTROL ClipControl; /* 0x10 */
    GPU_MODECONTROL ModeControl; /* 0x14 */
    GPU_VTECONTROL VteControl; /* 0x18 */
    unsigned int Unused; /* 0x1C */
    GPU_EDRAMMODECONTROL EdramModeControl; /* 0x20 */
    GPU_BLENDCONTROL BlendControl1; /* 0x24 */
    GPU_BLENDCONTROL BlendControl2; /* 0x28 */
    GPU_BLENDCONTROL BlendControl3; /* 0x2C */
} GPU_CONTROLPACKET;
