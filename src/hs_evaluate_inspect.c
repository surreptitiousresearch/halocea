/* hs_evaluate_inspect @0x83690298 — runtime of (inspect <expr>). On the initialize pass the argument is
 * evaluated into a frame slot. On the second pass the per-type inspector (hs_type_inspectors[type]) formats the
 * value into a local 1KB buffer, which is printed to the terminal when running in console or developer mode.
 * Always returns 0.
 *
 * Deviation: the decompiler lost the inspector's arguments (it calls through a void(void) cast); reconstructed
 * as inspector(value, buffer) per the engine's hs inspector signature. */

#include <stdint.h>
#include "headers/developer_mode.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/error_global_data.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(int thread_index, int value);
/* hs_type_inspectors: canonical 3-arg proto from blam_data_globals.h */
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void hs_evaluate_inspect(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;

    /* inlined hs_thread_stack_allocate(thread, sizeof(int)): reserve the value slot at the frame
     * data top, 4-aligned up. The compiled `if (slot - 1 > top) --slot` correction can never fire
     * (align-up advances at most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *value_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)0x3) /* align up to 4 */;
    if ( (uintptr_t)(value_slot - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --value_slot;
    frame->size = (int16_t)((unsigned char *)value_slot - frame->data + 4);

    int inspect_expression = frame->expression_index;
    int argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(inspect_expression).data).next_node_index;

    if ( initialize )
    {
        hs_evaluate(thread_index, argument, value_slot);
        return;
    }

    char buffer[1024];
    int16_t argument_type = HS_SYNTAX_NODE(argument).type;
    if ( hs_type_inspectors[argument_type] )
    {
        hs_type_inspectors[argument_type](argument_type, *value_slot, buffer); /* DB proto takes (value_type, value, buffer) */
        if ( hs_console_mode || error_globals.developer_mode >= _developer_mode_conprint_enabled )
            ship_terminal_printf((void *)0, buffer);
    }
    hs_return(thread_index, 0);
}
