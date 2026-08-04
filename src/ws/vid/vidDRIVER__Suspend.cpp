#include "../../headers/vidDRIVER.h"

// 0x82667BA0 -- ?Suspend@vidDRIVER@@UAAXXZ (virtual).
// Increment the suspend nesting counter. While >0 the driver is considered suspended and the
// thread-ownership hooks become no-ops (see AcquireThreadOwnership/ReleaseThreadOwnership).
void vidDRIVER::Suspend()
{
    ++this->suspendCounter;
}
