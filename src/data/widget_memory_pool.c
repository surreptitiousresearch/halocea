/* widget_memory_pool @ 0x8417C824 (.data, 4 bytes)
 * DB applied_types: stack_memory_pool *widget_memory_pool;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x841787F0 -> __medium_widget_memory_pool
 * data-init pointer (4 bytes) to the medium widget
 * stack allocator. Binary bytes 0x841787F0 big-endian = &_medium_widget_memory_pool
 * (stack_memory_pool_medium, whose leading member is a stack_memory_pool).
 */
#include "../headers/stack_memory_pool.h"

extern stack_memory_pool _medium_widget_memory_pool; /* 0x841787F0 */
stack_memory_pool *widget_memory_pool = &_medium_widget_memory_pool;
