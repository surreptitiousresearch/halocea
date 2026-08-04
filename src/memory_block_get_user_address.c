/* memory_block_get_user_address @0x83803128 — the user-data region begins immediately after the
 * 16-byte memory_block header. */

#include "headers/memory_block.h"

void * memory_block_get_user_address(memory_block *block)
{
    return block + 1;
}
