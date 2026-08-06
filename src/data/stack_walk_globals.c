/* stack_walk_globals @ 0x84185648 (.data, 20 bytes)
 * DB applied_types: _stack_walk_globals stack_walk_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 fixup                      = 0xFFFFFFFF
 *   +0x04 disregard_symbol_names     = 0x00
 *   +0x08 symbol_table               = 0x000000000000000000000000
 * data-init _stack_walk_globals (20-byte struct; ledger
 * size 24 overcounts by absorbing the following global). From the binary (big-endian):
 *   fixup = 0xFFFFFFFF -> -1; everything else zero.
 */
#include "../headers/stack_walk_globals.h"

_stack_walk_globals stack_walk_globals =
{
    -1,          /* fixup */
    0,           /* disregard_symbol_names */
    { 0, 0, 0 }, /* _pad05 */
    { 0, 0, 0 }  /* symbol_table */
};
