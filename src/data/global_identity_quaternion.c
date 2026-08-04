#include "../headers/real_quaternion.h"
/* points to named .data constant private_identity_quaternion {0,0,0,1} */
extern const real_quaternion private_identity_quaternion;
const real_quaternion *global_identity_quaternion = &private_identity_quaternion;
