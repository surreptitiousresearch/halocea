/* _medium_widget_memory_pool @ 0x841787F0 — data-init stack_memory_pool_medium (16436 bytes).
 * Only two fields are non-zero in the binary (big-endian):
 *   name             = 0x8211CE20 -> "widget_memory_pool"
 *   number_of_blocks = 0x00001000 -> 4096
 * Everything else (base_address, pool_size, next_block_index, statistics, locked, block
 * pointers, blocks[4096]) is zero-fill at load time. */
#include "../headers/stack_memory_pool.h"

stack_memory_pool_medium _medium_widget_memory_pool =
{
    "widget_memory_pool", /* name */
    0,                    /* base_address */
    0,                    /* pool_size */
    4096,                 /* number_of_blocks */
    0,                    /* next_block_index */
    { 0, 0, 0, 0, 0 },    /* statistics */
    0,                    /* locked */
    { 0, 0, 0 },          /* _pad29 */
    0,                    /* first_block */
    0,                    /* last_block */
    { 0 }                 /* blocks[4096] */
};
