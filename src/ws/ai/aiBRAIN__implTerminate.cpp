#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::implTerminate(float) @ 0x8325A8F8
// ?implTerminate@aiBRAIN@@UAAXM@Z (virtual)
//
// Terminate the brain by issuing Die("REMOVE"). The float time argument is unused (the brain removes
// itself immediately regardless of the requested delay).
void aiBRAIN::implTerminate(float /*time*/)
{
    dsTSTRING<char> reason("REMOVE"); // ctor body is UnsafeInit("REMOVE", -1, 0)
    Die(reason);
    // reason destructor self-releases at scope end.
}
