/* stack_walk_with_context @0x83811200 — capture up to 64 stack frames (via walk_stack_context when a
 * CPU context is supplied, walk_stack otherwise — DB proto confirms neither call actually takes the
 * context pointer; it only gates which capture routine runs) and, if an output stream is given, print each
 * captured return address with its resolved symbol name (or "?????" when symbols are unavailable/
 * disabled), from most-recent frame down to levels_to_ignore. */

#include <stdint.h>
#include <string.h>
#include "headers/win32_boundary.h"
#include "headers/stack_walk_globals.h"

typedef struct _iobuf _iobuf;

#include "headers/debug_symbol_table.h"
extern void walk_stack(unsigned int *routine_addresses, unsigned int number_of_levels, unsigned int ignore_levels, unsigned int *levels_dumped);
extern void walk_stack_context(unsigned int *routine_addresses, unsigned int number_of_levels, unsigned int ignore_levels, unsigned int *levels_dumped);
extern char * symbol_name_from_address(unsigned int fake_address, debug_symbol_table *symbol_table);
extern int fprintf(struct _iobuf *stream, const char *format, ...);

void stack_walk_with_context(struct _iobuf *error_stream, int16_t levels_to_ignore, _CONTEXT *context_pointer)
{
    unsigned int levels_dumped[4];
    unsigned int routine_addresses[80];

    memset(routine_addresses, 0, 256);

    if ( context_pointer )
        walk_stack_context(routine_addresses, 0x40u, levels_to_ignore, levels_dumped);
    else
        walk_stack(routine_addresses, 0x40u, levels_to_ignore, levels_dumped);

    if ( (int)(levels_dumped[0] - 1) >= levels_to_ignore )
    {
        int remaining = levels_dumped[0] - levels_to_ignore;
        unsigned int *frame = &routine_addresses[levels_dumped[0] - 1];

        do
        {
            if ( error_stream )
            {
                const char *symbol_name;
                if ( !stack_walk_globals.symbol_table.number_of_symbols || stack_walk_globals.disregard_symbol_names )
                    symbol_name = "?????";
                else
                    symbol_name = symbol_name_from_address(*frame, &stack_walk_globals.symbol_table);
                fprintf(error_stream, "%08lX %s\n", *frame, symbol_name);
            }
            --remaining;
            --frame;
        }
        while ( remaining );
    }
}
