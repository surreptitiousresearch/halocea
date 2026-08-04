#include "headers/simple_decompressor_definition.h"

/* Win32 import (kernel32 boundary; system <windows.h> collides with repo Win32 shim types). */
extern unsigned long WaitForSingleObjectEx(void *hHandle, unsigned long dwMilliseconds, int bAlertable);

void wait_for_raw_read(simple_decompressor_definition *self)
{
    WaitForSingleObjectEx(self->copy_stop_event, 0x2710u, 1);
    self->overlapped_in_use_flags[0] &= ~0x100u;
}
