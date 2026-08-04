#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiSQUAD_DESC::ParsePS @ 0x832A7F68
// ?ParsePS@aiSQUAD_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z
//
// Parse the squad-class descriptor from section `ps`: raise the descriptor priority to 70, then defer
// to the shared entity-descriptor parser. (`ps` is taken by value; entDESC::ParsePS receives its own
// copy, matching the binary's local psSECTION copy-construct/destruct.)
void aiSQUAD_DESC::ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr)
{
    SetPriority(70);                 // iaDESC::SetPriority (inherited, protected)
    entDESC::ParsePS(ps, hintErr);
}
