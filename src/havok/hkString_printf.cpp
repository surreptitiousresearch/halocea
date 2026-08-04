#include "../headers/havok/hkString.h"
#include <stdarg.h>
#include <stdio.h>

/* boundary — hkArray growth/shrink helpers (level-2 callees) */
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */
extern void hkArrayUtil__reduce(void *array, int sizeElem, char *inplaceMem, int newSize); /* hkArrayUtil::_reduce — args per ?_reduce@hkArrayUtil@@YAXPAXHPADH@Z */

#define HK_STRING_CAP(s) ((s)->m_string.m_capacityAndFlags & 0x3FFFFFFF)

/* hkString::printf — format into the string's own growable buffer, growing it
   with hkArrayUtil::_reserve until vsnprintf succeeds and fits, then trimming
   capacity with _reduce. DEVIATION: the PPC decompiler expands the fixed
   register-arg prologue into a long va_arg/spill sequence and miscounts the
   varargs; reconstructed with the standard <stdarg.h> forwarding to vsnprintf
   that the code actually implements. */
void hkString_printf(hkString *self, const char *fmt, ...)
{
    va_list args;
    int capacity;
    int written;

    /* Ensure a reasonable minimum working buffer. */
    if (HK_STRING_CAP(self) <= 0x3E)
    {
        int want = 2 * HK_STRING_CAP(self);
        if (want <= 256)
            want = 256;
        hkArrayUtil__reserve(&self->m_string, want, 1);
    }

    for (;;)
    {
        capacity = HK_STRING_CAP(self);
        va_start(args, fmt);
        written = vsnprintf(self->m_string.m_data, (size_t)capacity, fmt, args);
        va_end(args);

        if (written >= 0)
            break;

        /* Encoding error / truncation report of -1: grow and retry. */
        {
            int want = 2 * capacity;
            if (want <= 255)
                want = 255;
            if (HK_STRING_CAP(self) < want + 1)
                hkArrayUtil__reserve(&self->m_string, want + 1, 1);
            self->m_string.m_size = want + 1;
        }
    }

    if (written >= capacity)
    {
        /* Buffer was too small: grow to hold the full result and set size. */
        int needed = written + 1;
        if (HK_STRING_CAP(self) < needed)
        {
            int want = 2 * HK_STRING_CAP(self);
            if (needed >= want)
                want = needed;
            hkArrayUtil__reserve(&self->m_string, want, 1);
        }
        self->m_string.m_size = needed;
        return;
    }

    /* Fit: record size and give surplus capacity back. */
    self->m_string.m_size = written + 1;
    if (written + 1 <= HK_STRING_CAP(self))
        hkArrayUtil__reduce(&self->m_string, 1, 0, written + 1);
}
