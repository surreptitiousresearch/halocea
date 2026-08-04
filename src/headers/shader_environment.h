#pragma once
/* shader_environment — the environment (BSP/world) shader tag, the most common shader variant. Layout from the
 * database: a common _shader base (shader.h) followed by the environment-specific _shader_environment block.
 *
 * The rasterizer environment draw passes (diffuse/specular/reflection/self-illum) receive a pointer the
 * decompiler types as _shader* and reach these fields by indexing shader[N] (N = byte_offset/40). Reconstructed
 * source casts that pointer to shader_environment* and uses the named members below. */

#include "shader.h"
#include "_shader_environment.h"

typedef struct shader_environment
{
    _shader              shader;        /* 0x00 — common shader base */
    _shader_environment  environment;   /* 0x28 */
} shader_environment;                    /* 0x344 */
