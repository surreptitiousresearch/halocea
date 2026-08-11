// FUNCTION_INDEX entry: hkCriticalSection_construct @0x838EB698 (??0hkCriticalSection@@QAA@HH@Z)
#include "../headers/havok/hkReferencedObjectLock.h"

/* Win32/NT critical-section primitives (external boundary). */
extern void RtlInitializeCriticalSectionAndSpinCount(void *criticalSection, unsigned int spinCount);
extern void RtlLeaveCriticalSection(void *criticalSection);

/* The process-wide list head that threads all constructed sections together for
   debug enumeration (hkCriticalSection::SectionList::s_listHead). */
extern hkCriticalSection hkCriticalSection_SectionList_s_listHead;

/* hkCriticalSection::hkCriticalSection — construct with the given spin count and,
   when addToList is set, link this section at the front of the global section
   list (guarded by the list head's own section), then initialise the underlying
   NT critical section. */
void hkCriticalSection_construct(hkCriticalSection *self, unsigned int spinCount, int addToList)
{
    self->m_list.m_spinCount = (int)spinCount;
    self->m_list.m_prev = nullptr;
    self->m_list.m_next = nullptr;

    if (addToList)
    {
        hkCriticalSection *head = &hkCriticalSection_SectionList_s_listHead;
        hkCriticalSection *first;
        hkCriticalSection_enter(head);
        first = head->m_list.m_next;
        self->m_list.m_next = first;
        if (first)
            first->m_list.m_prev = self;
        self->m_list.m_prev = head;
        head->m_list.m_next = self;
        RtlLeaveCriticalSection(&head->m_section);
    }

    RtlInitializeCriticalSectionAndSpinCount(&self->m_section, spinCount);
}
