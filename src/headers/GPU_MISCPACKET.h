#pragma once

/* GPU_MISCPACKET - Xbox 360 D3D/GPU SDK type (external library boundary). DB-verified (types_members GPU_MISCPACKET). */
#include "GPU_AACONFIG.h"
#include "GPU_CONST.h"
#include "GPU_COPYCONTROL.h"
#include "GPU_COPYDESTINFO.h"
#include "GPU_COPYDESTPITCH.h"
#include "GPU_COPYFUNC.h"
#include "GPU_COPYMASK.h"
#include "GPU_COPYREF.h"
#include "GPU_DEBUGMISC0.h"
#include "GPU_DEBUGMISC1.h"
#include "GPU_OUTDEALLOCCONTROL.h"
#include "GPU_SAMPLECOUNTCONTROL.h"
#include "GPU_SCLINECONTROL.h"
#include "GPU_VERTEXREUSEBLOCKCONTROL.h"
#include "GPU_VTXCONTROL.h"

typedef struct GPU_MISCPACKET { // DB types_members GPU_MISCPACKET
    GPU_SCLINECONTROL ScLineControl; /* 0x00 */
    GPU_AACONFIG AaConfig; /* 0x04 */
    GPU_VTXCONTROL VtxControl; /* 0x08 */
    float GbVertClipAdj; /* 0x0C */
    float GbVertDiscAdj; /* 0x10 */
    float GbHorzClipAdj; /* 0x14 */
    float GbHorzDiscAdj; /* 0x18 */
    GPU_CONST VsConst; /* 0x1C */
    GPU_CONST PsConst; /* 0x20 */
    GPU_DEBUGMISC0 DebugMisc0; /* 0x24 */
    GPU_DEBUGMISC1 DebugMisc1; /* 0x28 */
    unsigned int Unused1[5]; /* 0x2C */
    unsigned int Unused2[2]; /* 0x40 */
    unsigned int AaMask; /* 0x48 */
    unsigned int Unused3[3]; /* 0x4C */
    GPU_VERTEXREUSEBLOCKCONTROL VertexReuseBlockControl; /* 0x58 */
    GPU_OUTDEALLOCCONTROL OutDeallocControl; /* 0x5C */
    GPU_COPYCONTROL CopyControl; /* 0x60 */
    unsigned int CopyDestBase; /* 0x64 */
    GPU_COPYDESTPITCH CopyDestPitch; /* 0x68 */
    GPU_COPYDESTINFO CopyDestInfo; /* 0x6C */
    unsigned int HiClear; /* 0x70 */
    unsigned int DepthClear; /* 0x74 */
    unsigned int ColorClear; /* 0x78 */
    unsigned int ColorClearLo; /* 0x7C */
    GPU_COPYFUNC CopyFunc; /* 0x80 */
    GPU_COPYREF CopyRef; /* 0x84 */
    GPU_COPYMASK CopyMask; /* 0x88 */
    unsigned int CopySurfaceSlice; /* 0x8C */
    GPU_SAMPLECOUNTCONTROL SampleCountControl; /* 0x90 */
    unsigned int SampleCountAddress; /* 0x94 */
} GPU_MISCPACKET;
