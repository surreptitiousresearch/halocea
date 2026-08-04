#pragma once
/* _shader_environment — the environment-specific block that follows the common _shader base inside a
 * shader_environment tag. Layout from the database (796 bytes). */

#include "tag_reference.h"
#include "shader_environment_diffuse_properties.h"
#include "shader_environment_self_illumination_properties.h"
#include "shader_environment_specular_properties.h"
#include "shader_environment_reflection_properties.h"

typedef struct _shader_environment
{
    unsigned __int16 flags;                                      /* 0x00 */
    __int16          type;                                       /* 0x02 */
    float            lens_flare_spacing;                         /* 0x04 */
    tag_reference    lens_flare;                                 /* 0x08 */
    int              unused[11];                                 /* 0x18 */
    shader_environment_diffuse_properties           diffuse;            /* 0x44 */
    shader_environment_self_illumination_properties self_illumination;  /* 0x158 */
    shader_environment_specular_properties          specular;           /* 0x254 */
    shader_environment_reflection_properties        reflection;         /* 0x2A8 */
} _shader_environment;                                            /* 796 bytes */
