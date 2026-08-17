/* ?SetStateProcNo@objOBJ@@QAAXHP6AHPAV1@PAX@Z@Z @0x826845F0 */
#include "objOBJ.h"
// 0x826845F0  ?SetStateProcNo@objOBJ@@QAAXHP6AHPAV1@PAX@Z@Z
// Thin forwarder: register a state proc whose condition callback must return false (enable=0).

void objOBJ::SetStateProcNo(int stateProc, StateCondProc cbCond)
{
    SetStateProc(stateProc, 0, cbCond);
}
