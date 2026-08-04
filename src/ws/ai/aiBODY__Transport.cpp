#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/fsm/fsmCB_POS_DIR_PAR.h"
#include "../../headers/animINST.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero

// aiBODY::Transport(const m3dV&) @ 0x832AB5D0
// ?Transport@aiBODY@@UAAXABUm3dV@@@Z
//
// Teleport the body to `pos`, keeping its current forward/up facing: build a pos/dir/up FSM
// callback block (facing queried from the animated instance) and forward it to the full transport.
void aiBODY::Transport(const m3dV &pos)
{
    fsmCB_POS_DIR_PAR par;
    par.pos = pos;
    par.dirZ = m3dVZero;
    par.dirY = m3dVZero;
    pBrain->pInst->GetDirUp(&par.dirY);
    pBrain->pInst->GetDir(&par.dirZ);
    Transport(par);
}
