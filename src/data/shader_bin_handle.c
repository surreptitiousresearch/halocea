/* shader_bin_handle @ 0x841758F8 (.data, 4 bytes)
 * DB applied_types: void *shader_bin_handle;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xFFFFFFFF
 */
#include "../headers/chicago_shaders.h"
/* DB size 8 but declared type is void*(4); trailing 4 bytes belong to a following global. */
/* bytes: FFFFFFFF = invalid handle sentinel */
void *shader_bin_handle = (void *)0xFFFFFFFFu;
