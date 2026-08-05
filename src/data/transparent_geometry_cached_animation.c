#include "../headers/render_animation.h"
/* transparent_geometry_cached_animation @ 0x8446C204 - .data zero-fill (IDA dword_8446C204, no PDB name).
 * Cached queued render-parameters animation snapshot; written with the rasterizer_memory_alloc_const(&params
 * ->animation, 8) result (disasm 0x8378D0FC `stw r3,(dword_8446C204-...)(r30)`), so a 4-byte pointer. */

const render_animation *transparent_geometry_cached_animation;
