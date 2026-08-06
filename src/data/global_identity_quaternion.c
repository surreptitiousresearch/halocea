/* global_identity_quaternion @ 0x84176F78 (.data, 4 bytes)
 * DB applied_types: const real_quaternion *global_identity_quaternion;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211458C -> private_identity_quaternion
 */
#include "../headers/real_quaternion.h"
/* points to named .data constant private_identity_quaternion {0,0,0,1} */
extern const real_quaternion private_identity_quaternion;
const real_quaternion *global_identity_quaternion = &private_identity_quaternion;
