/* render_debug_scripting @0x8368E188 — build and display the HaloScript debugging overlay (when the
 * debug_scripting switch is on): a table listing every live script thread with its name, remaining sleep time
 * in ticks, and current function. It walks the hs_thread_data array while the HS runtime is initialized,
 * skipping threads with a negative sleep field, formatting each row into a text buffer with "|t" tab separators,
 * then sets the column tab stops for the debug string renderer.
 *
 * DEVIATION: the inlined strlen scans and byte-copy strcat loops are written as strlen/strcat; the raw thread
 * datum (stride 536) is kept as offset accesses (sleep field @+8, current expression pointer @+16, empty-list
 * sentinel @+24). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/hs_runtime_globals.h"
#include "headers/hs_thread.h"
#include "headers/hs_stack_frame.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern int game_time_get(void);
extern const char * hs_thread_format(uint16_t thread_index);
extern const char * expression_get_function_name(uint16_t thread_index, int expression_index);
extern int sprintf_0(char *string, const char *format, ...);
/* strlen declared by <string.h> */
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);

void render_debug_scripting(void)
{
    if ( !debug_scripting )
        return;

    int16_t tab_stops[8];
    char buffer[2112];
    tab_stops[0] = 200;
    tab_stops[1] = 300;
    sprintf_0(buffer, "|n|n|nscript name|tsleep time|tfunction");

    for ( int i = data_next_index(hs_thread_data, -1); hs_runtime_globals.initialized;
            i = data_next_index(hs_thread_data, i) )
    {
        if ( i == -1 )
            break;

        hs_thread *thread = (hs_thread *)hs_thread_data->data + (uint16_t)i;
        if ( thread->sleep_until >= 0 )
        {
            sprintf_0(&buffer[strlen(buffer)], "|n%s|t", hs_thread_format(i));
            int sleep_remaining = thread->sleep_until ? thread->sleep_until - game_time_get() : 0;
            sprintf_0(&buffer[strlen(buffer)], "%d", sleep_remaining);
            strcat(buffer, "|t");

            hs_stack_frame *current_expression = thread->stack;
            if ( (uint8_t *)current_expression != thread->stack_data && thread->sleep_until != -2 )
                strcat(buffer, expression_get_function_name(i, current_expression->expression_index));
        }
    }

    buffer[1024] = 0;
    draw_string_set_tab_stops(tab_stops, 2);
    draw_string_set_tab_stops(tab_stops, 0);
}
