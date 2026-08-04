// DEVIATION: hand-written PowerPC lwarx/stwcx. lock-free primitive (see osLockedSet.cpp note),
// not a call to a Win32 InterlockedCompareExchange export.

// Atomic compare-and-swap: if *destination == comperand, stores `exchange` into *destination.
// Returns the value that was at *destination BEFORE the (attempted) swap, matching standard
// InterlockedCompareExchange semantics.
int osLockedCompareAndSwap(int *destination, int exchange, int comperand)
{
    int currentValue;
#ifdef _M_PPCBE
    bool reservationLost;
    while (true)
    {
        __asm
        {
            mfmsr     r10
            mtmsree   r13
            lwarx     r11, 0, r3
        }
        if (currentValue != comperand)
            break;
        __asm
        {
            stwcx.    r4, 0, r3
            mtmsree   r10
        }
        if (reservationLost)
            return currentValue;
    }
    __asm
    {
        stwcx.    r11, 0, r3
        mtmsree   r10
    }
#else
    // DEVIATION: x86 syntax-gate fallback for the PPC lwarx/stwcx. body above.
    currentValue = *destination;
    if (currentValue == comperand)
        *destination = exchange;
#endif
    return currentValue;
}
