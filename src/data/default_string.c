/* default_string @ 0x84177CF4 (.data, 4 bytes)
 * DB applied_types: unsigned __int16 *default_string;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x820309EC -> "" (empty string at target)
 * default_string (0x84177CF4). const wchar_t*, points to an empty wide string
 * (target 0x820309EC = 00 00 00 00).
 */
#include <wchar.h>
const wchar_t *default_string = L"";
