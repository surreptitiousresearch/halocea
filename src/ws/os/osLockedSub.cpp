// DEVIATION: hand-written PowerPC lwarx/stwcx. lock-free primitive (see osLockedSet.cpp note),
// not a call to a Win32 Interlocked export.

// Atomically subtracts `subtrahend` from *pVar and returns the value that was there BEFORE
// the subtraction (matches the disasm: the return register is loaded before the subtrahend is
// applied to the stored register).
int osLockedSub(int *pVar, int subtrahend)
{
    int previousValue;
#ifdef _M_PPCBE
    int difference;
    bool reservationLost;
    do
    {
        __asm
        {
            mfmsr     r8
            mtmsree   r13
            lwarx     r10, 0, r3
        }
        difference = previousValue - subtrahend; // r9 = r10 - subtrahend
        __asm
        {
            stwcx.    r9, 0, r3
            mtmsree   r8
        }
    } while (!reservationLost);
#else
    // DEVIATION: x86 syntax-gate fallback for the PPC lwarx/stwcx. body above.
    previousValue = *pVar;
    *pVar = previousValue - subtrahend;
#endif
    return previousValue;
}
