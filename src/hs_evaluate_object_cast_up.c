/* hs_evaluate_object_cast_up @0x83690458 — runtime of an object up-cast. One frame slot holds the evaluated
 * object reference. On the second pass the referenced object's type (object header -> object datum +180) is
 * tested against the type mask for this cast; a mismatch (or a null
 * reference) yields -1, otherwise the reference passes through unchanged.
 *
 * The one frame slot is reserved via the inlined hs_thread_stack_allocate idiom (see body comment). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/hs_function_index.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);

void hs_evaluate_object_cast_up(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;
    hs_stack_frame *frame = thread->stack;

    /* inlined hs_thread_stack_allocate(thread, sizeof(int)): reserve the result slot at the
     * frame data top, 4-aligned up. The compiled `if (slot - 4 > top) slot -= 4` correction
     * can never fire (align-up advances at most 3 bytes); kept verbatim for fidelity. */
    unsigned char *stack_top = &frame->data[frame->size];
    int *reference_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3);
    if ( (uintptr_t)reference_slot - 4 > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --reference_slot;
    frame->size = (int16_t)((unsigned char *)reference_slot - frame->data + 4);

    if ( initialize )
    {
        int cast_expression = frame->expression_index;
        int argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(cast_expression).data).next_node_index;
        hs_evaluate(thread_index, argument, reference_slot);
        return;
    }

    int reference = *reference_slot;
    if ( reference == -1 )
    {
        reference = -1;
    }
    else
    {
        object_datum *object_datum_ptr = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, reference)->datum;
        int16_t object_type = object_datum_ptr->object.type;
        if ( ((1 << object_type) & hs_object_type_masks[(int16_t)(function_index - _hs_function_inspect)]) == 0 )
            reference = -1;
    }
    hs_return(thread_index, reference);
}
