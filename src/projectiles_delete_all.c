/* projectiles_delete_all @0x837590E8 — delete every projectile object (type mask 0x20) in the world.
 *
 * DEVIATION: the decompiler types the result as void* (the trailing null iterator result); r3 at the
 * epilogue is only object_iterator_next's threaded value and the sole caller (cinematic_start
 * @0x83690F4C) ignores it — attested void. */

#include "headers/object_iterator.h"
#include "headers/object_type.h"

extern void object_delete(int object_index);

void projectiles_delete_all(void)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_projectile, 0);
    while ( object_iterator_next(&iterator) )
        object_delete(iterator.index);
}
