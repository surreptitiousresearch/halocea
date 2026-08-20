/* stack_memory_pool_unlock_block @0x838031D0 — clear a pool block's lock bit.
 *
 * DEVIATION: the parameters were `(stack_memory_pool *pool, memory_block *reference)` from the DB
 * prototype, which made the body clear bit 31 of `pool->name`. The instruction stream refutes it:
 * lwz r11,0(r3) @0x838031D0; clrlwi r10,r11,1 @0x838031D4; stw r10,0(r3) @0x838031D8;
 * blr @0x838031DC — instruction-identical to memory_block_unlock @0x838030D8, operating on r3
 * alone. r4 is never read (.attest/param_crosscheck.tsv: PHANTOM_TAIL), so the trailing parameter
 * is dropped and r3 is retyped `memory_block *`. Confirmed zero code xrefs.
 *
 * CAVEAT: dead in the shipped binary — see the note in headers/memory_block.h. */
#include "headers/memory_block.h"

void stack_memory_pool_unlock_block(memory_block *block)
{
    block->bits &= ~MEMORY_BLOCK_LOCKED_FLAG;
}
