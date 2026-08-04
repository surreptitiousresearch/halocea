#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiPERC_BASE
#include "../../headers/ws/ds/dsTSTRING.h"

// aiMIND::SetPar @ 0x83229010
// ?SetPar@aiMIND@@QAA_NABV?$dsTSTRING@D@@M@Z
//
// Fan a named float tunable out to the status block plus every mind module that has its own
// parameter table (isa / perception / vision). Every consumer is invoked (side effects must all
// run — no short-circuit); the call reports true when any of them recognised the key. The isa
// module's setter takes only the value (the key selection is internal to it).
bool aiMIND::SetPar(const dsTSTRING<char> &key, float val)
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    bool onStatus = brain->st.pointee->SetPar(key, val);
    bool onIsa    = isa.pointee->SetPar(val);
    bool onPerc   = perc.pointee->SetPar(key, val);
    bool onVision = vision.pointee->SetPar(key, val);

    if (onIsa)    return true;
    if (onPerc)   return true;
    if (onVision) return true;
    if (onStatus) return true;
    return false;
}
