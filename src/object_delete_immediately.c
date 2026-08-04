/* object_delete_immediately @0x836F2220 — delete an object and its children this frame: run the
 * initial recursive teardown pass, then the final recursive delete. */

#include <stdint.h>

extern void object_delete_initial_recursive(int object_index, uint8_t delete_siblings);
extern void object_delete_recursive(int object_index, uint8_t delete_siblings);

void object_delete_immediately(int object_index)
{
    object_delete_initial_recursive(object_index, 0);
    object_delete_recursive(object_index, 0);
}
