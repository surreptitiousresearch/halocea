/* ?SetStateProcYes@objOBJ@@QAAXHP6AHPAV1@PAX@Z@Z @0x826845E0 */
#include "objOBJ.h"
// 0x826845E0  ?SetStateProcYes@objOBJ@@QAAXHP6AHPAV1@PAX@Z@Z
// Thin forwarder: register a state proc whose condition callback must return true (enable=1).

void objOBJ::SetStateProcYes(int stateProc, StateCondProc cbCond)
{
    SetStateProc(stateProc, 1, cbCond);
}
