#include "osLOCK.h"

// 0x825E54C0 -- CAVEAT: disasm is exactly 3 instructions (li r11,0 / stb r11,0x20(r3) / blr,
// 12 bytes total) and only clears isInited. There is NO call to a Rtl*DeleteCriticalSection
// (or equivalent) here -- the underlying OS critical-section object is left as-is, not torn
// down. Reproduced verbatim; this is the shipped behavior, not a decompiler omission.
void osLOCK::Term()
{
    isInited = false;
}
