#include "../../headers/ws/ai/aiPLANNER_DESC.h"
#include "../../headers/ws/ia/iaDESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER_DESC::ParsePS(psSECTION, const dsTSTRING<char>&) @ 0x83163A08
// Parse the shared iaDESC config from `ps`, then pin this actor's default priority to 44.
// (The disassembly copy-constructs a temporary psSECTION for the by-value base call and destroys
// it on return — that is exactly the by-value `ps` parameter pass, reproduced here directly.)
void aiPLANNER_DESC::ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr)
{
    iaDESC::ParsePS(ps, hintErr);
    iaDESC::SetPriority(44);
}
