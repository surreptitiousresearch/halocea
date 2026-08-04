#include "../../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::~haloENGINE_CONTROL @ 0x823CAC60 — tear down in reverse member order:
// release pathToCheckpoints, curLevel, curCheckpoint (each a dsTSTRING<char> refcount-decrement taken
// under dsTSTRING<char>::processorLock, freeing the buffer at zero refs), then destroy the SSL object
// ref and finally the nested event dispatcher (whose own dtor unsubscribes from the event manager).
haloENGINE_CONTROL::~haloENGINE_CONTROL()
{
}
