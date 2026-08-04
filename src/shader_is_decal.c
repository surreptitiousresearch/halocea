/* shader_is_decal 0x837557F0 — true when the shader is a decal-style transparent shader that should be
 * routed through the decal pass. Only the transparent shader types (transparent_generic..transparent_meter,
 * 5..10) qualify; the answer is the shader's "decal" radiosity flag (bit 1, e.g.
 * _shader_transparent_generic_decal_bit) for most, the base flag (bit 0) for the type-10 (meter) default,
 * and never for transparent_water (8). The decal flag lives in the second shader block's radiosity flags. */

#include <stdint.h>
#include "headers/shader.h"
#include "headers/shader_type.h"

uint8_t shader_is_decal(const shader *shader)
{
    if ( shader && (unsigned int)(unsigned __int16)(shader->base.type - _shader_type_transparent_generic) <= 5 )
    {
        switch ( shader->base.type )
        {
            case _shader_type_transparent_generic:
            case _shader_type_transparent_chicago:
            case _shader_type_transparent_chicago_extended:
            case _shader_type_transparent_glass:
                return (shader[1].base.radiosity.flags >> 1) & 1;  /* decal_bit (1) */
            case _shader_type_transparent_water:
                return 0;
            default:  /* _shader_type_transparent_meter */
                return shader[1].base.radiosity.flags & 1;
        }
    }
    return 0;
}
