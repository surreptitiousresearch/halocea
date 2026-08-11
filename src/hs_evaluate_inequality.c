/* hs_evaluate_inequality @0x8368FA20 — runtime of (> a b) / (< a b) / (>= a b) / (<= a b). Both operands are
 * evaluated with the first operand's type, converted to float according to that type (real / long / short or
 * enum), and ordered. function_index 15 = '>', 16 = '<', 17 = '>=', else = '<='.
 *
 * Deviation: the decompiler renders the per-type (int -> float) conversions through overlapping __int64 temps
 * (the PowerPC integer-to-float idiom); reconstructed here as plain 32-bit operand conversions. The two operand
 * slots are 4 bytes each (value1 at +0, value2 at +4). The boolean is returned in the result int's leading
 * (big-endian) byte. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern int * hs_arguments_evaluate(int thread_index, int16_t formal_parameter_count, const int16_t *formal_parameters, uint8_t initialize);
extern void hs_return(int thread_index, int value);

void hs_evaluate_inequality(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_stack_frame *frame = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index)->stack;
    int call_expression = frame->expression_index;
    int first_argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(call_expression).data).next_node_index;
    int16_t operand_type = HS_SYNTAX_NODE(first_argument).type;

    parameter_types[0] = parameter_types[1] = operand_type;  /* DEVIATION: shared .data array @0x8441E64C, not a stack copy */
    void *values = hs_arguments_evaluate(thread_index, 2, parameter_types, initialize);
    if ( !values )
        return;

    float left, right;
    if ( parameter_types[0] == hs_type_real )      /* reloaded from the global at 0x8368FAD4 */
    {
        left = ((float *)values)[0];
        right = ((float *)values)[1];
    }
    else if ( parameter_types[0] == hs_type_long_integer )
    {
        left = (float)((int *)values)[0];
        right = (float)((int *)values)[1];
    }
    else                                /* short / enum */
    {
        left = (float)((int16_t *)values)[0];
        right = (float)((int16_t *)values)[2];   /* second slot at byte +4 */
    }

    unsigned char comparison;
    switch ( function_index )
    {
        case _hs_function_gt:  comparison = left > right;  break;   /* > */
        case _hs_function_lt:  comparison = left < right;  break;   /* < */
        case _hs_function_gte: comparison = left >= right; break;   /* >= */
        default:               comparison = left <= right; break;   /* <= (_hs_function_lte) */
    }

    int result = 0;
    *(unsigned char *)&result = comparison;
    hs_return(thread_index, result);
}
