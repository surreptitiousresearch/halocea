#include "../os/osTIMER2.h"

// High-resolution timer primitives — Saber os boundary (Xenon time-base backed).
extern "C" unsigned __int64 osGetPerfCounter(void);
extern "C" unsigned __int64 osGetPerfFrequancy(void);

// DEVIATION: the decompiler produced a garbled LODWORD/HIDWORD 64-bit-in-32-bit-GPR
// reconstruction (register-allocation artifact of the Xenon 64-bit-GPR/32-bit-pointer ABI).
// The disassembly is unambiguous: `ld r10,0(r11)` loads the full 64-bit `this->start`,
// `subf r9,r10,r3` subtracts it from the full 64-bit osGetPerfCounter() result — i.e. a
// plain 64-bit tick delta, not a 32-bit-halves operation. The result is then converted to
// float, multiplied by 1000.0 (constant 0x447A0000), and divided by the perf frequency —
// so this returns MILLISECONDS elapsed since Reset(), not seconds as CLAUDE.md guessed.
float osTIMER2::Snapshot()
{
    unsigned __int64 elapsedTicks = osGetPerfCounter() - start;
    float elapsedMs = (float)elapsedTicks * 1000.0f / (float)osGetPerfFrequancy();
    time = elapsedMs;
    return elapsedMs;
}
