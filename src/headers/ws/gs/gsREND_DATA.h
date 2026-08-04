#pragma once
#include "../msg/msgDATA.h"
// ws-engine gs: per-frame render pass descriptor handed to the render-debug hooks.
// DB-verified layout (types_members gsREND_DATA) — size 12: msgDATA base@0 (4), pCam@4,
// distortPass@8 (read by aiPLANNER::RenderDebugInfo at +0x08, lwz, disasm-verified 0x832471F8).

struct camCAMERA; // cam subsystem — camera (fwd, boundary pointer)

struct gsREND_DATA : msgDATA {
    camCAMERA *pCam;        // 0x04
    int        distortPass; // 0x08 nonzero during the distortion render pass
};
