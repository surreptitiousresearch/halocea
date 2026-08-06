/* global_devicetype @ 0x84174968 (.data, 4 bytes)
 * DB applied_types: _D3DDEVTYPE global_devicetype;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000001
 * global_devicetype (0x84174968). _D3DDEVTYPE, bytes 00 00 00 01 = D3DDEVTYPE_HAL (1).
 */
#include "headers/_D3DDEVTYPE.h"
_D3DDEVTYPE global_devicetype = (_D3DDEVTYPE)1;
