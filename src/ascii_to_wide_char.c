#include <stdint.h>
#include <stddef.h>
/* ascii_to_wide_char @0x8376D920 — zero-extends a single-byte ASCII character to wchar_t. */

wchar_t ascii_to_wide_char(uint8_t ascii)
{
    return (wchar_t)ascii;
}
