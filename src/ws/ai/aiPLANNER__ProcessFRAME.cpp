// aiPLANNER::ProcessFRAME @ 0x83246D10
// ?ProcessFRAME@aiPLANNER@@UAAXXZ
//
// Per-frame driver for the AI planner: fetch this frame's elapsed time from the iaIACTOR base and
// tick every owned AI subsystem in order, then run the debug frame and chain to the base
// iaIACTOR::ProcessFRAME. Wrapped in the "planner/total" profiling counter and a PIX event.
//
// The embedded subsystems (defCDT, coordinator, smr, bass, LOD, CheaVis, gloss, grass, stat) are
// stored inline in aiPLANNER as byte-accurate opaque members; they are reinterpret_cast to their
// real types to invoke each subsystem's tick (the established aiPLANNER member-access idiom).
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h" // also provides apCOUNTER_TIME_CALL_WRP

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent();

void aiPLANNER::ProcessFRAME()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/planner/total", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> profileScope(&localTimeCounter);

    osPIXBeginEvent("aiPLANNER::ProcessFRAME");

    float dt = reinterpret_cast<iaIACTOR *>(this)->GetElapsedTime();

    (&defCDT)->ProcessFrame(dt);
    navManager->ProcessFrame();
    penManager->ProcessFrame(dt);
    (&coordinator)->ProcessFrame(dt);
    (&smr)->UpdateTimers(dt);
    (&bass)->ProcessFrame(dt);
    (&LOD)->ProcessFrame(dt);
    (&CheaVis)->ProcessFRAME(dt);
    (&gloss)->ProcessFrame(dt);
    (&grass)->ProcessFrame(dt);
    (&stat)->ProcessFrame(dt);

    ProcessDebugFrame(dt);
    reinterpret_cast<iaIACTOR *>(this)->ProcessFRAME();

    osPIXEndEvent();
}
