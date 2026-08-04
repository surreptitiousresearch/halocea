/* hs_evaluate_equality @0x8368F8F0 — runtime of (= a b) / (!= a b). Both operands are evaluated (via
 * hs_arguments_evaluate) with the first operand's type, then compared byte-for-byte over hs_type_sizes[type].
 * function_index 13 (=) returns equal; 14 (!=) returns not-equal.
 *
 * The operand type is read from the first argument node (frame -> call expr -> name node -> first arg -> type).
 * Deviation: the boolean is returned in the result int's leading (big-endian) byte, matching the original's
 * HIBYTE store. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern int * hs_arguments_evaluate(int thread_index, int16_t formal_parameter_count, const int16_t *formal_parameters, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern const __int16 hs_type_sizes[];

void hs_evaluate_equality(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_stack_frame *frame = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index)->stack;
    int call_expression = frame->expression_index;
    int first_argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(call_expression).data).next_node_index;
    __int16 operand_type = HS_SYNTAX_NODE(first_argument).type;

    __int16 parameter_types[2] = { operand_type, operand_type };
    int *values = hs_arguments_evaluate(thread_index, 2, parameter_types, initialize);
    if ( !values )
        return;

    const unsigned char *first = (const unsigned char *)values;
    const unsigned char *second = (const unsigned char *)(values + 1);
    int diff = 0;
    __int16 size = hs_type_sizes[operand_type];
    for ( __int16 i = 0; i < size; ++i )
    {
        diff = first[i] - second[i];
        if ( diff != 0 )
            break;
    }

    unsigned char equal = (function_index == _hs_function_not_equal) ? (diff != 0) : (diff == 0);
    int result = 0;
    *(unsigned char *)&result = equal;
    hs_return(thread_index, result);
}
