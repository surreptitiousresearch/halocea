/* shader_type_is_vertex_lit @0x83755910 */
/* shader_type_is_vertex_lit — true for model (4) and transparent_glass (9). */

#include <stdint.h>
#include "headers/shader_type.h"

int shader_type_is_vertex_lit(int16_t shader_type)
{
    return shader_type == _shader_type_model || shader_type == _shader_type_transparent_glass;
}
