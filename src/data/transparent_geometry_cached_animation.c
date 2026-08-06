/* transparent_geometry_cached_animation @ 0x8446C204 (.data, 516 bytes)
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
 *   ... 356 further bytes elided; full hex in .sweep/data_image.tsv
 */
#include "../headers/render_animation.h"
/* transparent_geometry_cached_animation @ 0x8446C204 - .data zero-fill (IDA dword_8446C204, no PDB name).
 * Cached queued render-parameters animation snapshot; written with the rasterizer_memory_alloc_const(&params
 * ->animation, 8) result (disasm 0x8378D0FC `stw r3,(dword_8446C204-...)(r30)`), so a 4-byte pointer. */

const render_animation *transparent_geometry_cached_animation;
