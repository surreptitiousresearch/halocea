/* global_zero_angles2d @ 0x84176F70 (.data, 4 bytes)
 * DB applied_types: const real_euler_angles2d *global_zero_angles2d;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211453C -> "" (empty string at target)
 * .data data-init (const real_euler_angles2d *).
 * Points at shared .rdata zero-constant @ 0x8211453C = {yaw=0,pitch=0}.
 */
#include "../headers/real_euler_angles2d.h"
static const real_euler_angles2d _global_zero_angles2d = { { 0.0f, 0.0f } };
const real_euler_angles2d *global_zero_angles2d = &_global_zero_angles2d;
