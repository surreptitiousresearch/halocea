#include "../../headers/vidDRIVER.h"
#include "../ds/ds_assert_boundary.h"

// os: processor index the calling thread is currently running on (?osGetCurThreadProcessor@@YAHXZ).
extern int osGetCurThreadProcessor(); // boundary

// 0x82668F78 -- ?ReleaseThreadOwnership@vidDRIVER@@UAAXXZ (virtual).
// Release driver thread ownership on the outermost vidLOCK::Unlock. Asserts the driver is not
// suspended, then (only while not suspended) forwards the release to the underlying driver
// interface and clears the recorded owning processor.
void vidDRIVER::ReleaseThreadOwnership()
{
    osGetCurThreadProcessor();

    if (!IGNORE_STRONG_ASSERT)
    {
        osGetCurThreadProcessor();
        if (this->suspendCounter > 0)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                "!IsSuspended()",
                "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid.cpp",
                1121,
                empty_string);
    }

    osGetCurThreadProcessor();
    if (this->suspendCounter <= 0)
    {
        this->drvInterface->ReleaseThreadOwnership();
        this->acquiredThreadProcessor = -1;
    }
}
