/* eye_distance_to_fog_plane @ 0x8446C430 (.data, 4 bytes)
 * DB applied_types: float eye_distance_to_fog_plane;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * data-bss zero-fill; DB applied-type `float eye_distance_to_fog_plane;`. DB type is 4 bytes; ledger size 8 is the next-symbol gap heuristic (overcount) - DB authoritative.
 * A5 sweep flagged stfs/lfs at +0x4: that is the SEPARATE adjacent global rasterizer_fog_eye_blend_factor
 * (DB flt_8446C434 @ 0x8446C434), stored via the same anchor register in _rasterizer_environment_fog_begin
 * (disasm 0x8378FADC..0x8378FAFC: `stfs f,(eye_distance_to_fog_plane-0x8446C430)(r10)` then
 * `stfs f,(flt_8446C434-0x8446C430)(r10)`). Sibling-global access, not overflow. Type float correct. 2026-07-31.
 */
float eye_distance_to_fog_plane;
