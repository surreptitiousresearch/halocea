/* ?leave@hkCriticalSection@@QAAXXZ @0x838D3128 */
#include "../headers/havok/hkReferencedObjectLock.h"

/* Win32/NT critical-section primitive (external boundary). */
extern void RtlLeaveCriticalSection(void *criticalSection);

/* hkCriticalSection::leave — release the section. In the binary this is a thunk
   straight to RtlLeaveCriticalSection on the embedded _RTL_CRITICAL_SECTION. */
void hkCriticalSection_leave(hkCriticalSection *self)
{
    RtlLeaveCriticalSection(&self->m_section);
}
