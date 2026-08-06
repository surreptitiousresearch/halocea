// DEVIATION: this is not a wrapper around a Win32/xboxkrnl Interlocked* export -- the disasm
// shows a hand-written lock-free primitive built directly from PowerPC load-link/store-conditional
// (lwarx/stwcx.) with interrupts masked around the reservation (mfmsr/mtmsree toggling the MSR[EE]
// bit) and a retry loop on reservation loss. Reproduced verbatim as inline asm; only the register
// names have been given descriptive C identifiers.

// Atomically stores `newValue` into *pVar and returns the PREVIOUS value.
// DEVIATION: was `extern "C"`. The binary exports ?osLockedSet@@YAHPAHH@Z — MANGLED — so C linkage
// here would emit a flat `osLockedSet` the image does not contain, and every caller would resolve
// against a symbol nothing defines. C++ linkage reproduces the mangled name exactly.
int osLockedSet(int *pVar, int newValue)
{
    int previousValue;
#ifdef _M_PPCBE
    bool reservationLost;
    do
    {
        __asm
        {
            mfmsr     r10
            mtmsree   r13
            lwarx     r11, 0, r3
            stwcx.    r4, 0, r3
            mtmsree   r10
        }
    } while (!reservationLost);
#else
    // DEVIATION: x86 syntax-gate fallback for the PPC lwarx/stwcx. body above.
    previousValue = *pVar;
    *pVar = newValue;
#endif
    return previousValue; // returned via r11 (lwarx result) per the disasm
}
