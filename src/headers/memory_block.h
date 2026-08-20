#pragma once
/* memory_block — allocation header stored immediately before a stack-pool user pointer.
 * The low 31 bits of `bits` are the block size; the high bit (0x80000000) is the LOCK bit.
 *
 * The bit was previously named IN_USE. That reads backwards against the DB's own names for the
 * three accessors that touch it — memory_block_lock @0x838030C8 (oris r10,r11,0x8000),
 * memory_block_unlock @0x838030D8 (clrlwi r10,r11,1) and memory_block_is_locked @0x838030E8
 * (srwi r3,r11,31) — and against what stack_memory_pool_compact does with it: srwi r8,r9,31 /
 * cmplwi cr6,r8,0 / bne loc_83803330 @0x838032F4-0x838032FC skips the memmove for a block whose
 * bit 31 is set. A set bit means "pinned, do not relocate", i.e. locked.
 *
 * CAVEAT (documentation, not a defect): the compactor is dead by construction in this build. The
 * whole handle half of the stack_memory_pool API has zero code xrefs: pool_new_handle @0x83803A68,
 * pool_new_handle_clear @0x83803B10, pool_resize_handle @0x83803BB8, lock_handle @0x83803428,
 * unlock_handle @0x83803448, memory_block_lock @0x838030C8, memory_block_unlock @0x838030D8,
 * memory_block_is_locked @0x838030E8, stack_memory_pool_lock_block @0x838031B8 and
 * stack_memory_pool_unlock_block @0x838031D0 — the last five have no xref rows at all, not even a
 * .pdata unwind entry. pool_new_pointer_clear @0x83803DB8 is dead too. The only live entry points
 * are pool_new_pointer @0x83803D00 and pool_resize_pointer @0x83803E70 (four code xrefs each,
 * all UI widget code), and both SET
 * bit 31 on every block they hand out, so stack_memory_pool_compact walks the chain and moves
 * nothing, every time. The pool behaves as a first-fit allocator over the gaps between
 * permanently pinned blocks; the compaction code reads as though it runs, and it cannot. */

typedef struct memory_block
{
    unsigned int         bits;            /* 0x0 — size (& 0x7FFFFFFF) | locked (0x80000000) */
    unsigned int         index;           /* 0x4 — slot in the pool's block table */
    struct memory_block *previous_block;  /* 0x8 */
    struct memory_block *next_block;      /* 0xC */
} memory_block;                           /* 16 bytes */

/* `bits` accessors: the high bit marks the block locked, the low 31 bits are the block size. */
#define MEMORY_BLOCK_LOCKED_FLAG       0x80000000
#define MEMORY_BLOCK_IS_LOCKED(bits)   (((bits) & MEMORY_BLOCK_LOCKED_FLAG) != 0)
#define MEMORY_BLOCK_SIZE(bits)        ((bits) & 0x7FFFFFFF)
