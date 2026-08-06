/* sv_name_value @ 0x8417ED88 (.data, 32 bytes)
 * DB applied_types: wchar_t sv_name_value[16];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 0048 0061 006C 006F 0000 0000 0000 0000
 *   +0x0010: 0000 0000 0000 0000 0000 0000 0000 0000
 * bytes: UTF-16BE "Halo" then zero-fill (16 wchars)
 */
#include <stddef.h>   /* wchar_t */

wchar_t sv_name_value[16] = L"Halo";
