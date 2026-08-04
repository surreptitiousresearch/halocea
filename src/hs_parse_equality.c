/* hs_parse_equality @0x83779448 — parse (= a b) / (!= a b). The two operands must share a type: the first is
 * parsed untyped (type 0) and the second forced to the first's resolved type; if the first cannot be typed the
 * roles are swapped; failing both, both are parsed as real (type 6). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_compile_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t hs_get_parameter_indices(const char *function_name, int16_t count, int *result_indices, int expression_index);
extern uint8_t hs_parse(int expression_index, int16_t expected_type);

int hs_parse_equality(int16_t function_index, int expression_index)
{
    int operands[2];
    if ( !hs_get_parameter_indices(hs_function_table[function_index]->name, 2, operands, expression_index) )
        return 0;

    int left = operands[0];
    int right = operands[1];
    unsigned char failed;

    if ( hs_parse(left, hs_unparsed) )
    {
        failed = hs_parse(right, HS_SYNTAX_NODE(left).type) == 0;
    }
    else
    {
        if ( hs_compile_globals.__noop )
            return 0;
        if ( hs_parse(right, hs_unparsed) )
        {
            failed = hs_parse(left, HS_SYNTAX_NODE(right).type) == 0;
        }
        else
        {
            if ( hs_compile_globals.__noop || !hs_parse(left, hs_type_real) )
                return 0;
            failed = hs_parse(right, hs_type_real) == 0;
        }
    }

    return failed ? 0 : 1;
}
