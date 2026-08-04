#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dbg/dbgVAR_MANAGER.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"

// a8_debug.cpp file-scope (anonymous namespace) toggle mirrored from the "ui/ingame text/enable"
// console var each frame.
namespace { bool isRenderIngameText; }

// aiDEBUG::ProcessFrame @ 0x83196928  (?ProcessFrame@aiDEBUG@@QAAXXZ)
//
// Per-frame: cache the brain's current face origin/direction and refresh the ingame-text render
// toggle from the debug-var manager.
void aiDEBUG::ProcessFrame()
{
    aiBRAIN *pBrain = this->pBrain;
    aiSTATUS *st = pBrain->st.pointee;
    this->vOrgFace = st->cs.posFace;
    this->vDirFace = st->cs.dirFace;

    dsTSTRING<char> name;
    name.UnsafeInit("ui/ingame text/enable", -1, 0);
    dbgVAR_MANAGER *mgr = dbgVAR_MANAGER::GetManager();
    dbgVAR *var = mgr->FindFirst(name);
    if (var && var->typeId == 1)               // typeId 1 == bool-valued var
        isRenderIngameText = static_cast<dbgVAR_IMPL<bool, 1> *>(var)->value;
}
