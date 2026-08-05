/* rasterizer_fog_eye_blend_factor @ 0x8446C434 - .data zero-fill (IDA flt_8446C434, no PDB name).
 * Single-precision: _rasterizer_environment_fog_begin writes it with `stfs` at 0x8378FAEC / 0x8378FAFC /
 * 0x8378FB04. It is the global immediately after eye_distance_to_fog_plane (0x8446C430), which
 * src/data/eye_distance_to_fog_plane.c already documents as a separate sibling global. */

float rasterizer_fog_eye_blend_factor;
