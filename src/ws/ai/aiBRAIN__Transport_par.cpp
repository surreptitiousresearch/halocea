#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/fsm/fsmFSM_EVENT.h"
#include "../../headers/ws/fsm/fsmCB_POS_DIR_PAR.h"

// aiBRAIN::Transport(fsmCB_POS_DIR_PAR&) @ 0x832AED00
// ?Transport@aiBRAIN@@UAAXAAVfsmCB_POS_DIR_PAR@@@Z
//
// Instantly move the brain to the pos/dir/up described by `par`: fire the body FSM's "reset"
// callback with the pose block, then refresh the cached body coordinate system and notify the
// best-behaviour module of the teleport.
void aiBRAIN::Transport(fsmCB_POS_DIR_PAR &par)
{
    aiBODY_IFACE *bodyIface = body.pointee;
    bodyIface->ActivateBodyCB(&fsmFSM_EVENT::reset_(), &par);
    UpdateBodyCS();
    mind.best.pointee->NoticeTransported();
}
