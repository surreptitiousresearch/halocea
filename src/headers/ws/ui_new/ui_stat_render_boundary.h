#pragma once
#include "../rend/rendDRIVER.h"
// Boundary declarations for UI_STAT_RENDER__Flush.cpp: the render-driver / video-driver methods
// it calls into but does not itself reverse (large self-contained ws-engine classes, per the
// project's re-source conventions — only the call shape actually used here is modeled, at its
// DB-verified offset, per-consumer, matching the corpus's existing vidDRIVER-boundary headers
// such as txm_vid_boundary.h / vid_boundary.h).

// canonical DB-verified full layouts (the former per-consumer partial slices were
// consolidated): vidDIP_STAT (SetCurDip; ws/vid/vidDIP.h) and vidDRIVER (dipStat@0x290).
#include "../../vidDRIVER.h"

extern vidDRIVER *vidDriver;

// rendDRIVER::Configure / DrawIndexedPrimitive — render-state setup + an indexed-primitive draw
// call. Not reversed in this batch (rendDRIVER is a large self-contained boundary class — see
// rendDRIVER.h); only the exact call shape UI_STAT_RENDER::Flush uses is declared.
extern void rendDRIVER_Configure(rendDRIVER *self, int count, void *stateDesc,
        unsigned long long coordDescriptor, int coordSpace, int renderBlock, int arg7, int arg8);
extern void rendDRIVER_DrawIndexedPrimitive(rendDRIVER *self, void *vertices, int nVert,
        void *indices, int nTri, unsigned long long coordDescriptor, unsigned int flags);

// COORDSPACE_SCREEN / RENDERBLOCK_COLOR — rend enum constants passed to Configure.
#define COORDSPACE_SCREEN  0
#define RENDERBLOCK_COLOR  0

// Debug page-name byte used as vidDIP_STAT::SetCurDip's `id` argument (same global referenced as
// `dsStrongAssertMessage`/`empty_string` elsewhere in the corpus's boundary headers).
extern unsigned char empty_string;
