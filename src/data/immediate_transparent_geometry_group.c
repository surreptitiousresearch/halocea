/* immediate_transparent_geometry_group @ 0x8446C208 (.data, 512 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x00000000
 *   +0x0038 = 0x00000000
 *   +0x003C = 0x00000000
 *   +0x0040 = 0x00000000
 *   +0x0044 = 0x00000000
 *   +0x0048 = 0x00000000
 *   +0x004C = 0x00000000
 *   +0x0050 = 0x00000000
 *   +0x0054 = 0x00000000
 *   +0x0058 = 0x00000000
 *   +0x005C = 0x00000000
 *   +0x0060 = 0x00000000
 *   +0x0064 = 0x00000000
 *   +0x0068 = 0x00000000
 *   +0x006C = 0x00000000
 *   +0x0070 = 0x00000000
 *   +0x0074 = 0x00000000
 *   +0x0078 = 0x00000000
 *   +0x007C = 0x00000000
 *   +0x0080 = 0x00000000
 *   +0x0084 = 0x00000000
 *   +0x0088 = 0x00000000
 *   +0x008C = 0x00000000
 *   +0x0090 = 0x00000000
 *   +0x0094 = 0x00000000
 *   +0x0098 = 0x00000000
 *   +0x009C = 0x00000000
 *   ... 352 further bytes elided; full hex in .sweep/data_image.tsv
 * the singleton transparent_geometry_group used by
 * _rasterizer_model_transparent_geometry_submit for the immediate/no-queue (decal / active-camouflage)
 * draw path. data-bss zero-fill, DB type transparent_geometry_group (168 bytes).
 * P4 sweep flagged accesses at +0xA8 (== struct size) through +0x1B8: these fall in the 512-byte gap to
 * local_parameters (0x8446C408) and are SEPARATE rasterizer local_* globals (DB dword_8446C2B0,
 * unk_8446C3A8, unk_8446C3C0) reached off the local_parameters base register (r30=0x8446C418), not off
 * this struct's base. Sibling-block access, not overflow. Type transparent_geometry_group is correct.
 */
#include "../headers/transparent_geometry_group.h"

transparent_geometry_group immediate_transparent_geometry_group;
