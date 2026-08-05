#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/ds_assert_boundary.h" // empty_string (the shared "" global)

// aiBRAIN::ProcessINITDead @ 0x83179700
// ?ProcessINITDead@aiBRAIN@@QAAXXZ
//
// Bring up a brain that spawned dead: mark it dead with an empty reason string, then run the mind's
// dead-spawn init path. (byte_8200155A is the shared empty-string constant.)
void aiBRAIN::ProcessINITDead()
{
    dsTSTRING<char> reason(empty_string);
    Die(reason);
    mind.ProcessINITDead();
}
