/* dangerous_projectiles_near_player @0x83759770 — true if any projectile object (type mask 0x20)
 * currently exists in the world. */

#include <stdint.h>
#include "headers/object_iterator.h"
#include "headers/object_type.h"

/* attested uint8_t: 4/4 callers clrlwi r3,24; callee computes 0/1 via addic/subfe */
uint8_t dangerous_projectiles_near_player(void)
{
    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_projectile, 0);
    return object_iterator_next(&iterator) != 0;
}
