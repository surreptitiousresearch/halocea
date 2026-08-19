/* hs_script_evaluate @0x8368E6C0 — drive evaluation of a top-level HaloScript script on a thread. On the
 * `initialize` call it notifies the HCEX bridge (hcex_on_start_script) and kicks off evaluation of the
 * script's root expression (script datum +0x24) into a freshly bump-allocated 4-byte result slot on the
 * thread's stack frame. On every later call it simply returns the value sitting in that slot.
 *
 * Threads are 536-byte data_array data; the stack-frame pointer is at thread+16 and the frame keeps its fill
 * size as an int16 at frame+12. The allocation idiom (round up, step back if it overshot) matches the rest of
 * the HaloScript interpreter (see hs_arguments_evaluate). Script datums are 92 bytes in scenario hs_scripts. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/hs_script.h"
#include "headers/hs_stack_frame.h"
#include "headers/hs_thread.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_return(int thread_index, int value);
extern void hcex_on_start_script(void *script);

void hs_script_evaluate(int16_t script_index, int thread_index, uint8_t initialize)
{
    hs_stack_frame *frame = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index)->stack;
    hs_script *script = &((hs_script *)global_scenario->hs_scripts.address)[script_index];

    /* inlined hs_thread_stack_allocate(thread, sizeof(int)): reserve the result slot at the frame
     * data top, 4-aligned up. The compiled `if (slot - 1 > top) --slot` correction can never fire
     * (align-up advances at most 3 bytes); kept verbatim for fidelity. */
    unsigned char *stack_top = &frame->data[frame->size];
    int *result_slot = (int *)(((uintptr_t)stack_top + 3) & ~(uintptr_t)3);
    if ( (uintptr_t)(result_slot - 1) > (uintptr_t)stack_top )   /* dead branch (shipped) */
        --result_slot;
    frame->size = (int16_t)((unsigned char *)result_slot - frame->data + 4);

    if ( initialize )
    {
        hcex_on_start_script(script);
        hs_evaluate(thread_index, script->root_expression_index, result_slot);
    }
    else
    {
        hs_return(thread_index, *result_slot);
    }
}
