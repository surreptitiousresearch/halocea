/* stack_walk_globals @ 0x84185648 — data-init _stack_walk_globals (20-byte struct; ledger
 * size 24 overcounts by absorbing the following global). From the binary (big-endian):
 *   fixup = 0xFFFFFFFF -> -1; everything else zero. */
#include "../headers/stack_walk_globals.h"

_stack_walk_globals stack_walk_globals =
{
    -1,          /* fixup */
    0,           /* disregard_symbol_names */
    { 0, 0, 0 }, /* _pad05 */
    { 0, 0, 0 }  /* symbol_table */
};
