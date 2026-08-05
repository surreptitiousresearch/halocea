#include <stdint.h>
/* transparent_geometry_cached_sorted_index @ 0x8446C2A0 - .data zero-fill (IDA dword_8446C2A0, no PDB name).
 * The decal/camouflage singleton group's cached sorted index; _rasterizer_model_transparent_geometry_submit
 * stores -1 into it as a 32-bit word (disasm 0x8378CE38: `li r28,-1` / `stw r28,(dword_8446C2A0-...)(r30)`). */

int transparent_geometry_cached_sorted_index;
