#include "../headers/render_lighting.h"
/* transparent_geometry_cached_lighting @ 0x8446C1C0 - .data zero-fill (IDA dword_8446C1C0, no PDB name).
 * Cached queued render_lighting snapshot; written with rasterizer_memory_alloc_const(&params->lighting, 0x74)
 * (disasm 0x8378D0E0), so a 4-byte pointer. */

const render_lighting *transparent_geometry_cached_lighting;
