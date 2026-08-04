/* error_string @ 0x84469590 -- third-order .data. Shared 256-wchar scratch buffer that ustrerror()
 * formats strerror() text into via usnprintf. 512 bytes, zero-initialised (bss). */
#include <wchar.h>
wchar_t error_string[256];
