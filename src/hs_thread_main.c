/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hs_thread_main @0x83690778 — run one HaloScript thread until it blocks, sleeps, or finishes. A fresh
 * thread (empty stack) first starts its scenario script and evaluates the root expression. Then the
 * interpreter loop pops stack frames: each frame's syntax node is either a script call (push a new frame and
 * either start the callee or return its value, depending on the tail-call flag) or a built-in function
 * (dispatch through hs_function_table). The loop stops when the thread sleeps (negative wake time), is
 * waiting on a future game time, or the runtime is shutting down. When the stack drains, the thread is
 * deleted (runtime_evaluate thread, type 2), or — for a startup or dormant scenario script (script_type 0/1) —
 * parked by setting its wake time to -1 (which stops it auto-running; hs_runtime_update only runs wake time >= 0).
 * A continuous script (script_type 2) instead misses that branch, keeps wake time 0, and re-runs next tick.
 *
 * Threads are 536-byte data; thread+16 holds the current stack-frame pointer, thread+24 is the stack base,
 * thread+8 the wake time, byte thread+2 the thread/sleep type, and bit 0 of byte thread+3 the start-vs-return
 * flag. Frame+12 holds the frame size. The raw offsets and the result-slot allocation math are reproduced
 * verbatim from the compiled code (matching hs_evaluate's frame handling). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/hs_runtime_globals.h"
#include "headers/hs_function_definition.h"
#include "headers/hs_thread.h"
#include "headers/hs_script.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/hs_thread_type.h"
#include "headers/hs_thread_flags.h"
#include "headers/hs_script_type.h"
#include "headers/blam_data_globals.h"

extern hs_runtime_globals_t hs_runtime_globals;

extern void hcex_on_start_script(const char *script_name);
extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t game_in_progress(void);
extern int game_time_get(void);
extern void datum_delete(data_array *data, int index);

/* Carve a 4-byte, 4-byte-aligned result slot just past the frame's used size and record the new
 * frame size. Inlined hs_thread_stack_allocate(thread, sizeof(int)): the compiled
 * `if (slot - 1 > top) --slot` correction can never fire (align-up advances at most 3 bytes);
 * kept verbatim for fidelity. */
static int *hs_thread_push_result_slot(hs_thread *thread)
{
    hs_stack_frame *frame = thread->stack;
    unsigned char *stack_top = &frame->data[frame->size];
    int *slot = (int *)(((unsigned int)stack_top + 3) & ~3u) /* align up to 4 */;
    if ((unsigned int)(slot - 1) > (unsigned int)stack_top)   /* dead branch (shipped) */
        --slot;
    frame->size = (__int16)((unsigned char *)slot - frame->data + 4);
    return slot;
}

void hs_thread_main(int thread_index)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
    hcex_cur_hs_thread = thread_index;
    unsigned char thread_type = thread->type;
    hs_script *script = nullptr;
    hs_runtime_globals.executing_thread_index = thread_index;
    if (!thread_type)
        script = &((hs_script *)global_scenario->hs_scripts.address)[thread->script_index];

    hs_stack_frame *stack_base = (hs_stack_frame *)thread->stack_data;
    thread->sleep_until = 0;

    if (thread->stack == stack_base)
    {
        hcex_on_start_script(script->name);
        thread->stack->size = 0;
        int *result_slot = hs_thread_push_result_slot(thread);
        hs_evaluate(thread_index, script->root_expression_index, result_slot);
    }

    while (thread->stack != stack_base)
    {
        if (thread->sleep_until < 0
            || (game_in_progress() && thread->sleep_until > game_time_get())
            || !hs_runtime_globals.initialized)
        {
            break;
        }

        hs_stack_frame *frame = thread->stack;
        char start_flag = thread->flags & (1u << _hs_thread_in_function_call_bit);
        int node_index = (unsigned __int16)frame->expression_index;
        frame->size = 0;
        thread->flags &= ~(1u << _hs_thread_in_function_call_bit);

        hs_syntax_node *node = DATA_ARRAY_ELEMENT(hs_syntax_data, hs_syntax_node, node_index);
        if ((node->flags & (1u << _hs_syntax_node_script_bit)) != 0) /* node is a script call */
        {
            hs_script *callee =
                &((hs_script *)global_scenario->hs_scripts.address)[node->___u1.function_index];
            int *result_slot = hs_thread_push_result_slot(thread);
            if (start_flag)
            {
                hcex_on_start_script(callee->name);
                hs_evaluate(thread_index, callee->root_expression_index, result_slot);
            }
            else
            {
                hs_return(thread_index, *result_slot);
            }
        }
        else /* built-in function */
        {
            __int16 function_index = node->___u1.function_index;
            hs_function_table[function_index]->evaluate(function_index, thread_index, start_flag);
        }
    }

    if (thread->stack == stack_base)
    {
        if (thread->type)
        {
            if (thread->type == hs_thread_type_runtime_evaluate)
                datum_delete(hs_thread_data, thread_index);
        }
        else if (script->script_type == _hs_script_startup || script->script_type == _hs_script_dormant) /* park */
        {
            thread->sleep_until = -1;
            hs_runtime_globals.executing_thread_index = -1;
            return;
        }
    }

    hs_runtime_globals.executing_thread_index = -1;
}
