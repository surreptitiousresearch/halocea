/* stack_walk_dispose @0x83811188 — tears down the debug symbol table used for stack-walk resolution
 * (freeing its string storage and symbol array via the pool's dlFree) and resets stack_walk_globals to
 * its unloaded state. */

#include "headers/stack_walk_globals.h"

extern void dlFree(void *ptr);

void stack_walk_dispose(void)
{
    stack_walk_globals.fixup = -1;
    stack_walk_globals.disregard_symbol_names = 0;
    if ( stack_walk_globals.symbol_table.string_storage )
        dlFree(stack_walk_globals.symbol_table.string_storage);
    if ( stack_walk_globals.symbol_table.symbols )
        dlFree(stack_walk_globals.symbol_table.symbols);
    stack_walk_globals.symbol_table.number_of_symbols = 0;
    stack_walk_globals.symbol_table.string_storage = 0;
    stack_walk_globals.symbol_table.symbols = 0;
}
