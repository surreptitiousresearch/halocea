/* depths_of_hell @ 0x84184C50 — data-init real_plane3d (16 bytes).
 * Initializer reconstructed from the binary (big-endian):
 *   n = {0.0, 0.0, 1.0}   (0x00000000, 0x00000000, 0x3F800000)
 *   d = -256.0            (0xC3800000)
 * The ledger classified this data-bss, but the binary carries nonzero init bytes. */
#include "../headers/real_plane3d.h"

real_plane3d depths_of_hell = { { 0.0f, 0.0f, 1.0f }, -256.0f };
