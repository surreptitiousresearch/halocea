/* hs_runtime_evaluate @ 0x83690D28 — evaluate a single hs (HaloScript) expression on a freshly
 * allocated script thread. For an immediate (non-sleeping) result the value is returned
 * directly; if the expression yields/sleeps the thread is handed to hs_thread_main and -1 is
 * returned. Returns -1 on any failure.
 *
 * The 536-byte hs_thread datum is initialized by raw field offset (its struct was not
 * reconstructed): +16 = stack-frame pointer (set to +24), +4 = result/parent (-1),
 * +2 = state byte (2 = running), +20 = evaluation result. */

#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_thread_type.h"
#include "headers/hs_thread_flags.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_thread_main(int thread_index);


int hs_runtime_evaluate(int expression_index)
{
    int thread_index;
    hs_thread *thread;

    if ( !hs_runtime_globals.initialized )
        return -1;
    if ( expression_index == -1 )
        return -1;

    thread_index = datum_new(hs_thread_data);
    if ( thread_index == -1 )
        return -1;

    thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    thread->stack = (hs_stack_frame *)thread->stack_data;   /* stack frame pointer -> inline frame */
    thread->stack->parent = 0;
    thread->stack->size = 0;
    thread->stack->expression_index = -1;
    thread->script_index = -1;
    thread->type = hs_thread_type_runtime_evaluate;
    thread->flags = 0;
    thread->sleep_until = 0;

    hs_evaluate(thread_index, expression_index, &thread->result);

    if ( thread->flags & (1u << _hs_thread_in_function_call_bit) )
    {
        hs_thread_main(thread_index);
        return -1;
    }
    return thread->result;   /* +20 = evaluation result word */
}
