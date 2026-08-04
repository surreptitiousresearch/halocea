#include "../../headers/ws/ai/aiCACHER_DESC.h"
#include "../../headers/ws/ia/iaDESC.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCACHER_DESC::ParsePS(psSECTION, const dsTSTRING<char>&) @ 0x83163A58
// Identical shape to aiPLANNER_DESC::ParsePS but with default priority 40.
void aiCACHER_DESC::ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr)
{
    iaDESC::ParsePS(ps, hintErr);
    iaDESC::SetPriority(40);
}
