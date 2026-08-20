/* stack_memory_pool_lock_block @0x838031B8 — lock a pool block and hand back its user address.
 *
 * DEVIATION: the parameter was typed `stack_memory_pool *pool` (the DB prototype says
 * `int *stack_memory_pool_lock_block(stack_memory_pool *pool, memory_block *reference)`), which
 * produced the meaningless `pool->name |= 0x80000000` and `return &pool->next_block_index`.
 * The instruction stream refutes the prototype and wins. The function is six instructions:
 * lwz r10,0(r3) @0x838031B8; mr r11,r3 @0x838031BC; addi r3,r3,0x10 @0x838031C0;
 * oris r9,r10,0x8000 @0x838031C4; stw r9,0(r11) @0x838031C8; blr @0x838031CC.
 * It reads only r3, sets bit 31 of its first dword, and returns r3 + 0x10. That is
 * memory_block_lock (0x838030C8: lwz r11,0(r3) / oris r10,r11,0x8000 / stw r10,0(r3) — the same
 * three instructions) followed by memory_block_get_user_address (0x83803128: addi r3,r3,0x10),
 * so r3 is a `memory_block *` and 0x10 is sizeof(memory_block). r4 is never referenced; the
 * second prototype parameter is phantom (.attest/param_crosscheck.tsv already flags the twin
 * function's r4 PHANTOM_TAIL). Confirmed zero code xrefs, so nothing in the corpus calls this
 * with the old signature.
 *
 * CAVEAT: dead in the shipped binary — see the note in headers/memory_block.h. */
#include "headers/memory_block.h"

void *stack_memory_pool_lock_block(memory_block *block)
{
    block->bits |= MEMORY_BLOCK_LOCKED_FLAG;
    return block + 1;
}
