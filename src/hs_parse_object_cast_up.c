/* hs_parse_object_cast_up @0x83779AF0 — parse the single argument of an object up-cast operator, expecting an
 * object designator (type 37). */

#include <stdint.h>
#include "headers/hs_function_definition.h"
#include "headers/hs_type.h"

extern uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_object_cast_up(int16_t function_index, int expression_index)
{
    int argument[1];
    if ( hs_get_parameter_indices(hs_function_table[function_index]->name, 1, argument, expression_index) )
        return hs_parse(argument[0], hs_type_object);
    return 0;
}
