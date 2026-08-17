/* ?enter@hkCriticalSection@@QAAXXZ @0x838EB570 */
#include "../headers/havok/hkReferencedObjectLock.h"

/* Win32/NT critical-section primitives (external boundary). */
extern int  RtlTryEnterCriticalSection(void *criticalSection);
extern void RtlEnterCriticalSection(void *criticalSection);

/* hkCriticalSection::enter — acquire the section. Fast path is a non-blocking
   try; only on contention does it fall back to the blocking enter.
   DEVIATION: when contended and the per-thread Havok monitor stream is active the
   original brackets the blocking wait with "TtCriticalLock"/"Et" timer records
   pushed onto that thread's hkMonitorStream (reached via the r13 thread pointer).
   That profiling instrumentation is elided here; the lock semantics are identical. */
void hkCriticalSection_enter(hkCriticalSection *self)
{
    if (!RtlTryEnterCriticalSection(&self->m_section))
        RtlEnterCriticalSection(&self->m_section);
}
