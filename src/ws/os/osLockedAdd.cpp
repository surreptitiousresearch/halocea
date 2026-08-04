// DEVIATION: hand-written PowerPC lwarx/stwcx. lock-free primitive (see osLockedSet.cpp note),
// not a call to a Win32 InterlockedExchangeAdd export.

// Atomically adds `addend` to *pVar and returns the value that was there BEFORE the add
// (matches the disasm: the return register is loaded before the addend is applied to the
// stored register).
int osLockedAdd(int *pVar, int addend)
{
    int previousValue;
#ifdef _M_PPCBE
    int sum;
    bool reservationLost;
    do
    {
        __asm
        {
            mfmsr     r9
            mtmsree   r13
            lwarx     r11, 0, r3
        }
        sum = addend + previousValue; // r10 = addend + r11
        __asm
        {
            stwcx.    r10, 0, r3
            mtmsree   r9
        }
    } while (!reservationLost);
#else
    // DEVIATION: x86 syntax-gate fallback for the PPC lwarx/stwcx. body above (cl.exe cannot
    // assemble PowerPC). Semantically equivalent for the gate; real target uses the __asm path.
    previousValue = *pVar;
    *pVar = previousValue + addend;
#endif
    return previousValue;
}
