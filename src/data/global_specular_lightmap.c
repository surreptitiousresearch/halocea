/* global_specular_lightmap @ 0x844A5B80 (.data, 4 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 */
#include "../headers/bitmap_data.h"
/* global_specular_lightmap @ 0x844A5B80 - .data zero-fill (IDA dword_844A5B80, no PDB name).
 * _rasterizer_environment_specular_lightmap_begin stores the incoming bitmap_data pointer (disasm 0x837A5CE0)
 * or 0 (0x837A5CFC) here, next to the DB-named local_lightmap_has_no_data (0x844A5B7C). */

bitmap_data *global_specular_lightmap;
