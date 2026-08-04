#include "dsTSTRING.h"

extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary — dlmalloc
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);                // boundary — CRT

// dsTSTRING<char>::UnsafeInit @ 0x823B0420  (protected — IAA mangle)
// Initialise this (uninitialised) string from `s`, reserving `reserve` extra characters of capacity
// beyond the measured length. `len` caps how many characters of `s` are considered (-1 = unlimited,
// stopping at the first NUL). Three outcomes:
//   * s == null and len != 0 -> adopt the shared empty-string singleton.
//   * measured text is empty AND reserve == 0 -> adopt the shared empty-string singleton.
//   * otherwise -> allocate a private buffer (header + measuredLen + reserve + terminator) and copy.
// Both singleton-adoption paths inline dsTSTRING<char>::UnsafeInitEmpty; expressed here as a call.
void dsTSTRING<char>::UnsafeInit(const char *s, int len, int reserve)
{
    this->pBuffer = nullptr;

    if (!s && len != 0) {
        this->UnsafeInitEmpty(); // adopt the process-wide shared empty string
        return;
    }

    if (*s == 0 && reserve == 0) {
        this->UnsafeInitEmpty(); // empty text, no reserve -> shared empty string
        return;
    }

    // Measure the character count (bounded by `len`, stopping at the first NUL).
    int cap = (len == -1) ? 0x7FFFFFFF : len;
    int measuredLen = 0;
    while (measuredLen < cap && s[measuredLen])
        ++measuredLen;

    // Capacity = measured + reserve, guarding against signed overflow (clamp back to measuredLen).
    int capacity = measuredLen + reserve;
    if (capacity < measuredLen)
        capacity = measuredLen;

    dsTSTRING_BUF_HEADER<char> *buf = (dsTSTRING_BUF_HEADER<char> *)dlMalloc(
        capacity + 16, // header (12) + capacity chars + NUL terminator
        "d:\\projects\\code\\common\\incl.sys\\ds\\ds_string.hpp",
        0x3B0);
    buf->strLen = measuredLen;
    buf->refCount = 1;
    buf->maxStrLen = capacity;
    buf->str[measuredLen] = 0;
    this->pBuffer = buf;
    memcpy(buf->str, s, measuredLen);
}
