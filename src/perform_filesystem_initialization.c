/* perform_filesystem_initialization @0x83734298 — kick off filesystem init on a background thread; if
 * thread creation fails, run it synchronously on the calling thread instead. */

#include <stdint.h>
#include "headers/widget_globals.h"

extern uint8_t create_thread(uint16_t flags, unsigned int (__fastcall *function)(void *),
    void *function_input, thread **thread_reference);
extern unsigned int filesystem_initialization_thread_proc(void *input);

unsigned int perform_filesystem_initialization(void)
{
    widget_globals.processing_inhibited = 1;
    widget_globals.filesystem_check_result = 0;

    unsigned int result = create_thread(0, filesystem_initialization_thread_proc, 0, &widget_globals.initialization_thread);

    if ( !(uint8_t)result )
    {
        widget_globals.initialization_thread = 0;
        result = filesystem_initialization_thread_proc(0);
        widget_globals.processing_inhibited = 0;
    }
    return result;
}
