// FUNCTION_INDEX entry: osLockedIncrement @0x825E54D8 (?osLockedIncrement@@YAHPAH@Z)
// DEVIATION: hand-written PowerPC lwarx/stwcx. lock-free primitive (see osLockedSet.cpp note),
// not a call to a Win32 InterlockedIncrement export.

// Atomically increments *pVar and returns the NEW value.
int osLockedIncrement(int *pVar)
{
    int newValue;
#ifdef _M_PPCBE
    bool reservationLost;
    do
    {
        __asm
        {
            mfmsr     r10
            mtmsree   r13
            lwarx     r11, 0, r3
        }
        newValue = newValue + 1; // r11 + 1, stored back to r11 per the disasm
        __asm
        {
            stwcx.    r11, 0, r3
            mtmsree   r10
        }
    } while (!reservationLost);
#else
    // DEVIATION: x86 syntax-gate fallback for the PPC lwarx/stwcx. body above.
    newValue = *pVar + 1;
    *pVar = newValue;
#endif
    return newValue;
}
