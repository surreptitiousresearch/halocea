#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"

// aiBRAIN::ProcessTERM @ 0x831762C0
// ?ProcessTERM@aiBRAIN@@UAAXXZ  (virtual)
//
// Terminate the brain: run the instant teardown, chain to the base entity termination, then drop
// this brain's SSL cross-references.
void aiBRAIN::ProcessTERM()
{
    TermInstant();
    entENTITY::ProcessTERM(); // base subobject termination (decompiler: entENTITY::ProcessTERM(&aiBRAIN_IFACE))
    RemoveSSLCrossRefs();
}
