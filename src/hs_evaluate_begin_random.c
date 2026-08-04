/* hs_evaluate_begin_random @0x8368ED60 — runtime of (begin_random ...). Evaluates each argument exactly once in
 * a random order. Three frame allocations are pushed: a 2-byte argument count, a bitmask (one bit per argument,
 * cleared on initialize) tracking which arguments have run, and a 4-byte result slot. Each pass picks the next
 * not-yet-used argument starting from a random offset, evaluates it, and marks it; once every argument has run
 * the last result is returned.
 *
 * thread+16 = active stack frame; frame+4 = current expression index; frame+12 = frame size (int16). Raw bump
 * allocation and the random-probe arithmetic are reproduced verbatim. */

#include <stdint.h>
#include "headers/hs_syntax_node.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void *memset(void *dst, int value, unsigned int count);

void hs_evaluate_begin_random(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_thread *thread = (hs_thread *)hs_thread_data->data + (unsigned __int16)thread_index;

    /* inlined hs_thread_stack_allocate x3: reserve slots at the frame data top, aligned up.
     * The compiled `if (slot - elem > top) slot -= elem` correction can never fire (align-up
     * advances at most 3 bytes); kept verbatim for fidelity. */
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    __int16 *count = (__int16 *)(((unsigned int)stack_top + 1) & ~0x1u);   /* 2-byte slot */
    if ( (unsigned int)(count - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --count;
    frame->size = (__int16)((unsigned char *)count - frame->data + 2);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    char *used_mask = (char *)(((unsigned int)stack_top + 3) & ~0x3u);
    if ( (unsigned int)(used_mask - 4) > (unsigned int)stack_top )   /* dead branch (shipped) */
        used_mask -= 4;
    frame->size = (__int16)((unsigned char *)used_mask - frame->data + 4);

    frame = thread->stack;
    stack_top = &frame->data[frame->size];
    int *result = (int *)(((unsigned int)stack_top + 3) & ~0x3u);
    if ( (unsigned int)(result - 1) > (unsigned int)stack_top )   /* dead branch (shipped) */
        --result;
    int probe_count = 0;   /* v15: number of used slots skipped this pass */
    frame->size = (__int16)((unsigned char *)result - frame->data + 4);

    int begin_expression = frame->expression_index;
    int first_argument = HS_SYNTAX_NODE(HS_SYNTAX_NODE(begin_expression).data).next_node_index;

    if ( initialize )
    {
        int node = first_argument;
        for ( *count = 0; node != -1; ++*count )
            node = HS_SYNTAX_NODE(node).next_node_index;
        memset(used_mask, 0, 4 * ((*count + 31) >> 5));
    }

    __int16 total = *count;
    unsigned int *seed = get_global_random_seed_address();
    int random_start = seed_random_range(seed, 0, total);
    int n = *count;
    if ( n > 0 )
    {
        int offset = 0;
        int mask_offset;
        int bit;
        int chosen;
        while ( 1 )
        {
            mask_offset = 4 * ((__int16)((random_start + offset) % n) >> 5);
            bit = 1 << (((random_start + offset) % n) & 0x1F);
            chosen = (__int16)((random_start + offset) % n);
            if ( (bit & *(int *)&used_mask[mask_offset]) == 0 )
                break;
            probe_count = (__int16)(offset + 1);
            offset = probe_count;
            if ( probe_count >= *count )
                goto done;
        }
        int node = first_argument;
        for ( ; chosen > 0; node = HS_SYNTAX_NODE(node).next_node_index )
            chosen = (__int16)(chosen - 1);
        hs_evaluate(thread_index, node, result);
        *(int *)&used_mask[mask_offset] |= bit;
    }
done:
    if ( (__int16)probe_count == *count )
        hs_return(thread_index, *result);
}
