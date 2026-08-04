#pragma once

/* shader_model tag block — the two-member wrapper the DB (and reference) define:
 * { _shader shader @0x00 (40 bytes); _shader_model model @0x28 (400 bytes) }.
 * Consumers reach the model-shader fields through `->model.<field>` and the common
 * base through `->shader.<field>`. Layout DB-verified via types_members. */

#include "_shader.h"
#include "_shader_model.h"

typedef struct shader_model
{
    _shader       shader; /* 0x000 */
    _shader_model model;  /* 0x028 */
} shader_model;           /* 0x1B8 (440 bytes) */
