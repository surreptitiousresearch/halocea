#include "../headers/real_matrix4x3.h"
/* transparent_geometry_cached_node_matrices @ 0x8446C2B0 - .data zero-fill (IDA dword_8446C2B0, no PDB name).
 * Cached queued node-matrix array; written with rasterizer_memory_alloc_const(params->skinning.node_matrices,
 * 0x34 * node_matrix_count) (disasm 0x8378D0C8), so a 4-byte pointer to 52-byte real_matrix4x3 elements. */

const real_matrix4x3 *transparent_geometry_cached_node_matrices;
