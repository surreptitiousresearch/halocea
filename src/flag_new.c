/* flag_new @0x8380F408 — allocate and initialize a runtime "flag" (cloth) object instance from a flag tag
 * definition. Rejects definitions that are too large (cell count >= 225, width >= 40) or lack a blue shader by
 * marking the datum a no-op; otherwise it clears the datum, seeds every cloth vertex to the world origin with
 * zero velocity, zeroes each interior cell's tesselation level, and computes the attachment and trailing
 * shapes. Returns the new flag datum index, or -1 on a bad definition index / allocation failure.
 *
 * DEVIATION: the decompiler renders flag_set_trailing_shape's two args as uninitialized locals (v14/v13);
 * disasm (0x8380F5C0/0x8380F5C4) shows no argument reload between the two shape calls and both functions'
 * prototypes are (flag_definition*, flag_datum*), so trailing_shape reuses the same (definition, flag) pair. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/flag_limits.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void flag_set_attachment_shape(flag_definition *flag_definition, flag_datum *flag);
extern void flag_set_trailing_shape(flag_definition *flag_definition, flag_datum *flag);

int flag_new(int flag_definition_index)
{
    if ( flag_definition_index == -1 )
        return -1;

    flag_definition *definition = TAG_GET(flag_definition, flag_definition_index);
    int result = datum_new(flag_data);
    if ( result == -1 )
        return -1;

    flag_datum *flag = DATA_ARRAY_ELEMENT(flag_data, flag_datum, result);

    if ( definition->height * definition->width >= MAXIMUM_FLAG_VERTEX_AREA
      || definition->width >= MAXIMUM_FLAG_HEIGHT   /* DB name is HEIGHT; guards the `width` field per the binary */
      || definition->shader_blue.index == -1 )
    {
        flag->__noop = 1;
    }
    else
    {
        flag->definition_index = flag_definition_index;
        flag->__noop = 0;
        flag->initialized = 0;
        flag->object_index = -1;
        flag->first_attachment.n[2] = 0.0f;
        flag->first_attachment.n[1] = 0.0f;
        flag->first_attachment.n[0] = 0.0f;

        if ( definition->width > 0 )
        {
            int column = 0;
            do
            {
                int height = definition->height;
                for ( int row = 0; row < height; row = (int16_t)(row + 1) )
                {
                    flag->vertices[column * height + row].p = *global_origin3d;
                    flag->vertices[column * height + row].v = *global_zero_vector3d;
                    if ( column < definition->width - 1 )
                    {
                        int interior_height = definition->height - 1;
                        if ( row < interior_height )
                            flag->cells[interior_height * column + row].tesselation = 0;
                    }
                    height = definition->height;
                }
                column = (int16_t)(column + 1);
            }
            while ( column < definition->width );
        }

        flag_set_attachment_shape(definition, flag);
        flag_set_trailing_shape(definition, flag);
        return result;
    }

    return result;
}
