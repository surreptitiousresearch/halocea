/* hs_macro_function_strings_evaluate @0x8368E998 — collect the evaluated string (value) arguments of a
 * variadic HaloScript macro one at a time on a thread. On `initialize` it bump-allocates four slots in the
 * thread's stack frame — a 4-byte count, a 128-byte (32-entry) value array, a 2-byte cursor, and a 4-byte
 * "current argument node" pointer — and seeds the node pointer to the call's first actual argument. On each
 * call it evaluates the current argument into the array and advances; it returns 0 ("more to do") until the
 * argument list is exhausted or 32 values have been collected, at which point it writes the count and array
 * pointer through the out-parameters and returns 1.
 *
 * Threads are 536-byte data_array data; the stack-frame pointer is at thread+16 and the frame keeps its fill
 * size as an int16 at frame+12. The allocation idiom matches the rest of the HaloScript interpreter (see
 * hs_arguments_evaluate). */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

extern void hs_evaluate(int thread_index, int expression_index, int *destination);

uint8_t hs_macro_function_strings_evaluate(int thread_index, uint8_t initialize, int *count_out, const char ***strings_out)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)thread_index;
    int done = 1;

    /* slot 1: collected-value count (4-byte aligned int).
     * inlined hs_thread_stack_allocate x4: reserve slots at the frame data top, aligned up. The
     * compiled `if (slot - elem > top) slot -= elem` correction can never fire (align-up advances
     * at most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *count = (int *)(((unsigned int)stack_top + 3) & ~0x3u); /* align up to 4 */
    if ( (unsigned int)(count - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --count;
    frame->size = (int16_t)((unsigned char *)count - frame->data + 4);

    /* slot 2: 32-entry value array (128 bytes) */
    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    char *values = (char *)(((unsigned int)stack_top + 3) & ~0x3u); /* align up to 4 */
    if ( (unsigned int)(values - 4) > (unsigned int)stack_top )   /* dead branch (shipped) */
        values -= 4;
    frame->size = (int16_t)((unsigned char *)values - frame->data + 128);

    /* slot 3: argument cursor (2-byte aligned int16) */
    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int16_t *cursor = (int16_t *)(((unsigned int)stack_top + 1) & ~0x1u); /* align up to 2 */
    if ( (unsigned int)(cursor - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --cursor;
    frame->size = (int16_t)((unsigned char *)cursor - frame->data + 2);

    /* slot 4: current-argument node index (4-byte aligned int) */
    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *current_argument = (int *)(((unsigned int)stack_top + 3) & ~0x3u); /* align up to 4 */
    if ( (unsigned int)(current_argument - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --current_argument;
    frame->size = (int16_t)((unsigned char *)current_argument - frame->data + 4);

    if ( initialize )
    {
        *cursor = 0;
        int call_node = (thread->stack)->expression_index;
        *current_argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(call_node).data).next_node_index;
        *count = 0;
        memset(values, 0, 0x80u);
    }

    if ( *current_argument != -1 && *cursor < 32 )
    {
        int value;
        hs_evaluate(thread_index, *current_argument, &value);
        done = 0;
        *current_argument = HS_SYNTAX_NODE(*current_argument).next_node_index;
        ((int *)values)[*count] = value;   /* 4-byte slots in the 4-aligned frame array */
        ++*cursor;
        ++*count;
    }

    if ( (unsigned char)done )
    {
        *count_out = *count;
        *strings_out = (const char **)values;
    }
    return done;
}
