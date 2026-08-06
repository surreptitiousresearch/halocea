/* translate_funcs @ 0x84184C98 (.data, 16 bytes)
 * DB applied_types: void (__fastcall *translate_funcs[2][2])(flying_camera *);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x837C43A0 -> translate_flying_to_orbiting
 *   +0x000C = 0x837C4428 -> translate_orbiting_to_flying
 * data-init 2x2 flying-camera mode-transition hook table (16 bytes).
 * Reconstructed from the binary (big-endian pointer words):
 *   [0][0] = 0            [0][1] = 0
 *   [1][0] = 0x837C43A0 -> translate_flying_to_orbiting
 *   [1][1] = 0x837C4428 -> translate_orbiting_to_flying
 * Consumers (editor_camera_*.c) reference this as void (*[][2])(flying_camera *).
 * The ledger classified this data-bss, but the binary carries nonzero init bytes.
 */
#include "../headers/flying_camera.h"

extern void translate_flying_to_orbiting(flying_camera *camera); /* 0x837C43A0 */
extern void translate_orbiting_to_flying(flying_camera *camera); /* 0x837C4428 */

void (*translate_funcs[2][2])(flying_camera *camera) =
{
    { 0, 0 },
    { translate_flying_to_orbiting, translate_orbiting_to_flying }
};
