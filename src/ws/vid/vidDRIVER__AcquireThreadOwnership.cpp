#include "../../headers/vidDRIVER.h"
#include "../ds/ds_assert_boundary.h"

// os: processor index the calling thread is currently running on (?osGetCurThreadProcessor@@YAHXZ).
extern int osGetCurThreadProcessor(); // boundary

// 0x82668EE0 -- ?AcquireThreadOwnership@vidDRIVER@@UAAXXZ (virtual).
// Take driver thread ownership on the outermost vidLOCK::Lock. Asserts the driver is not
// suspended, then (only while not suspended) records the current thread's processor and forwards
// ownership acquisition to the underlying driver interface.
void vidDRIVER::AcquireThreadOwnership()
{
    if (!IGNORE_STRONG_ASSERT)
    {
        osGetCurThreadProcessor();
        if (this->suspendCounter > 0)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                "!IsSuspended()",
                "D:\\Projects\\code\\common\\src.sys\\drv\\video\\vid.cpp",
                1106,
                empty_string);
    }

    osGetCurThreadProcessor();
    if (this->suspendCounter <= 0)
    {
        this->acquiredThreadProcessor = osGetCurThreadProcessor();
        this->drvInterface->AcquireThreadOwnership();
    }
}
