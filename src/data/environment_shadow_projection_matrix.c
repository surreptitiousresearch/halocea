/* environment_shadow_projection_matrix @ 0x844A5AE0 — copy of the current object's shadow projection
 * matrix, stored by _rasterizer_environment_shadow_begin (memcpy 0x34) and read back by
 * _rasterizer_environment_shadow_draw. data-bss zero-fill, DB type real_matrix4x3 (52 == 0x34 bytes).
 * (Resolves the char[0x34]/const real_matrix4x3 decl conflict: the memcpy size 0x34 equals
 * sizeof(real_matrix4x3) and the draw side reads .forward/.left/.up/.position — it is a real_matrix4x3.) */
#include "../headers/real_matrix4x3.h"

real_matrix4x3 environment_shadow_projection_matrix;
