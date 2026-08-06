/* face_mapping_inverse_table @ 0x821282E0 (.rdata, 12 bytes)
 * DB applied_types: const __int16 face_mapping_inverse_table[6];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 0000 0002 0001 0003 0004 0005
 * .rdata, 12 bytes = 6 x int16_t, one per cube-map face.
 * rasterizer_xbox_bitmap_rebuild_hardware_format uses it to remap a cubemap's source face slices
 * into Xbox hardware face order: {0, 2, 1, 3, 4, 5} — i.e. hardware faces 1 and 2 swap, the rest
 * are identity. Initializer reconstructed from the binary (big-endian 16-bit halves of
 * 0x00000002, 0x00010003, 0x00040005).
 * DEVIATION: src/rasterizer_xbox_bitmap_rebuild_hardware_format.c:13 declares this
 * `extern const int face_mapping_inverse_table[]`. That is wrong on both counts — the elements are
 * 16-bit and there are 6 of them, so a 4-byte-strided read of face 3..5 runs off the end of the
 * 12-byte object. The element width is settled by the disassembly at 0x837E3D5C/0x837E3D84:
 * `slwi r24, r20, 1` (index * 2) then `lhzx r11, r24, r21` with r21 = face_mapping_inverse_table,
 * and confirmed by the DB applied type `const __int16 face_mapping_inverse_table[6]`.
 * The consumer decl is left alone here; declaration normalization is owned elsewhere.
 */
#include <stdint.h>

const int16_t face_mapping_inverse_table[6] = { 0, 2, 1, 3, 4, 5 };
