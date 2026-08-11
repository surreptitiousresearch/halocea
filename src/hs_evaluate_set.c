/* hs_evaluate_set @0x8368F290 — runtime of (set <global> <value>). On the first pass (initialize) it starts
 * evaluating the value expression directly into the target global's storage slot, releasing any prior object
 * reference held by an object-typed global. On the second pass it reconciles the write, takes a new object
 * reference if needed, and returns the new value.
 *
 * Thread stack frame is at thread+16; the active expression index is at frame+4, the frame size (int16) at
 * frame+12. The single value slot pushed for the evaluated RHS is popped here (raw bump arithmetic reproduced
 * faithfully). Global storage is 8 bytes per global with the value at +4; external globals index by
 * HS_GLOBAL_DESIGNATOR_TO_INDEX, scenario globals add hs_external_global_count. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_thread.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_datum.h"
#include "headers/hs_type.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_global_get_type(int16_t designator);
extern void hs_global_reconcile_write(int16_t global_designator);
extern int hs_global_evaluate(int16_t global_designator);
extern void object_list_add_reference(int object_list_index);
extern void object_list_remove_reference(int object_list_index);
extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(int thread_index, int value);

void hs_evaluate_set(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;
    hs_stack_frame *frame = thread->stack;

    int set_expression = frame->expression_index;
    int name_node = HS_SYNTAX_NODE(set_expression).data;
    int variable_node = HS_SYNTAX_NODE(name_node).next_node_index;
    int global_designator = HS_SYNTAX_NODE(variable_node).data;

    /* inlined hs_thread_stack_allocate(thread, sizeof(int)): (re)reserve the 4-byte evaluated-value
     * slot at the frame data top, 4-aligned up. The compiled `if (slot - 1 > top) --slot` correction
     * can never fire (align-up advances at most 3 bytes); kept verbatim for fidelity. */
    unsigned char *stack_top = &frame->data[frame->size];
    int *popped = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3); /* align up to 4 */
    if ( (uintptr_t)(popped - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --popped;
    frame->size = (int16_t)((unsigned char *)popped - frame->data + 4);

    int16_t type = hs_global_get_type(global_designator);
    if ( !initialize )
    {
        hs_global_reconcile_write(global_designator);
        if ( type == hs_type_object_list )   /* 23 = hs_type_object_list (hs_type.h) */
            object_list_add_reference(hs_global_evaluate(global_designator));
        hs_return(thread_index, hs_global_evaluate(global_designator));
    }
    else
    {
        if ( type == hs_type_object_list )   /* 23 = hs_type_object_list (hs_type.h) */
            object_list_remove_reference(hs_global_evaluate(global_designator));
        int storage_index = HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator);
        if ( !HS_GLOBAL_IS_EXTERNAL(global_designator) )
            storage_index += hs_external_global_count;
        int value_expression = HS_SYNTAX_NODE(variable_node).next_node_index;
        hs_evaluate(thread_index, value_expression,
                    &DATA_ARRAY_ELEMENT(hs_global_data, hs_global_datum, storage_index)->value.long_value);
    }
}
