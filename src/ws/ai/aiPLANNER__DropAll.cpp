// aiPLANNER::DropAll @ 0x83247430
// ?DropAll@aiPLANNER@@IAAXXZ
//
// Per-level teardown helper: drop the transient state of every owned AI subsystem and clear the
// anim-death smart-object list. The inline subsystem members are byte-accurate opaque storage and
// are reinterpret_cast to their real types (established aiPLANNER member-access idiom).
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/nav/navPENMAN.h"

void aiPLANNER::DropAll()
{
    (&fe)->Clear();
    (&chaser)->DropAll();
    (&coordinator)->DropAll();
    (&defCDT)->DropAll();
    penManager->DropAll();
    reinterpret_cast<aiTEAMS_SYS *>(&teams)->DropAll();
    (&LOD)->DropAll();
    (&CheaVis)->DropAll();
    (&gloss)->DropAll();
    (&grass)->DropAll();
    wah.DropAll();
    animDeathSmtObjects.Clear();
}
