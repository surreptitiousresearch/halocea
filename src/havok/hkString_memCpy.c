#include <string.h>

/* hkString::memCpy — a thunk straight to the CRT memcpy (the binary tail-calls
   memcpy; there is no additional logic). */
void hkString_memCpy(void *dst, const void *src, size_t n)
{
    memcpy(dst, src, n);
}
