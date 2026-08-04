/* hs_parse_inequality @0x83779598 — parse (< a b) / (> a b) / (<= a b) / (>= a b). Like equality, the operands
 * must share a type, but that type must be orderable: an enum range (types 32..36) or a numeric type
 * (real/short/long, 6..8). The first parseable operand fixes the comparison type; failing both, real (6) is
 * used. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

/* orderable == an enum type (first..last_hs_enum_type) or a numeric type (real/short/long) */
#define HS_TYPE_IS_ORDERABLE(t) (((t) >= first_hs_enum_type && (t) <= last_hs_enum_type) \
                              || ((t) >= hs_type_real && (t) <= hs_type_long_integer))

int hs_parse_inequality(int16_t function_index, int expression_index)
{
    int operands[2];
    if ( !hs_get_parameter_indices(hs_function_table[function_index]->name, 2, operands, expression_index) )
        return 0;

    int left = operands[0];
    int right = operands[1];
    __int16 comparison_type;
    int other;

    if ( hs_parse(left, hs_unparsed) && (comparison_type = HS_SYNTAX_NODE(left).type, HS_TYPE_IS_ORDERABLE(comparison_type)) )
    {
        other = right;
    }
    else
    {
        if ( hs_compile_globals.__noop )
            return 0;
        if ( hs_parse(right, hs_unparsed)
          && (comparison_type = HS_SYNTAX_NODE(right).type, HS_TYPE_IS_ORDERABLE(comparison_type)) )
        {
            other = left;
        }
        else
        {
            if ( hs_compile_globals.__noop || !hs_parse(left, hs_type_real) )
                return 0;
            comparison_type = hs_type_real;
            other = right;
        }
    }

    return hs_parse(other, comparison_type) ? 1 : 0;
}
