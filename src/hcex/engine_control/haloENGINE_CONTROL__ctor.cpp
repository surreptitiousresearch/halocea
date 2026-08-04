#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::haloENGINE_CONTROL @ 0x823CAF50 — construct the ws-engine's handle onto the
// legacy Blam engine. The nested event dispatcher default-constructs with no owner (owner bound later
// by InitSettings/SetProduct), the SSL object ref default-constructs, and the three dsTSTRING<char>
// members default-construct to the process-wide shared empty string. isCoop starts solo.
//
// (The decompiler renders each dsTSTRING default-construct inline as an UnsafeInitEmpty +
// refcount-bump against the shared empty-string static; expressed here as the member default-ctors
// the compiler generated it from.)
haloENGINE_CONTROL::haloENGINE_CONTROL()
    : eventDispatcher(),
      sslObject(),
      curCheckpoint(),
      curLevel(),
      pathToCheckpoints(),
      isCoop(false)
{
}
