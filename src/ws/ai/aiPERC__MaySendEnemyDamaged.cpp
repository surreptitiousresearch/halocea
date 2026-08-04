#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"

// aiPERC::MaySendEnemyDamaged @ 0x8329CDD0
// ?MaySendEnemyDamaged@aiPERC@@IBA_NXZ
//
// The accumulated-damage report may be sent only while the unit is not actively firing.
bool aiPERC::MaySendEnemyDamaged() const
{
    aiWEAPON_BASE *wpn = pBrain->body.pointee->wpn.pointee;
    return !wpn->IsShootingNow();
}
