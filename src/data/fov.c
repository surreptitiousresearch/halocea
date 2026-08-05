/* fov @ 0x82126570 — .rdata, 8 bytes = 2 x float. The editor debug camera's per-mode field of
 * view, indexed by the global `camera_mode` (int16_t). Initializer reconstructed from the
 * binary (big-endian words):
 *   +0x0000 0x3F9C61AA -> 1.2217305f (70 degrees in radians, bit-exact)
 *   +0x0004 0x3F9C61AA -> 1.2217305f
 * The 2-element extent is exact: the next named .rdata address is the "speed is now x%f"
 * literal @0x82126578. Element type confirmed float, not int: editor_camera_get_field_of_view
 * @0x837C44C8 returns the element straight out of `lfsx f1, r7, r9` with no conversion.
 *
 * DEVIATION: src/editor_camera_get_field_of_view.c declares `extern float fov[]` without const.
 * The object is in .rdata (segment perm 4, read-only), so the original was const-qualified; the
 * definition follows the segment and the consumer decl is untouched. */

const float fov[2] = { 1.2217305f, 1.2217305f };
