#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/ds_assert_boundary.h" // dsStrongAssertMessage (empty-string global)

// aiBRAIN::ProcessINITDead @ 0x83179700
// ?ProcessINITDead@aiBRAIN@@QAAXXZ
//
// Bring up a brain that spawned dead: mark it dead with an empty reason string, then run the mind's
// dead-spawn init path. (byte_8200155A is the shared empty-string constant.)
void aiBRAIN::ProcessINITDead()
{
    dsTSTRING<char> reason(dsStrongAssertMessage);
    Die(reason);
    mind.ProcessINITDead();
}
