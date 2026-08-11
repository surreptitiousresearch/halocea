/* hs_object_destroy_containing @0x837F8048 — destroy every object whose name contains the given substring. */

#include <stdint.h>

extern void hs_object_iterate_names_containing(const char *name_string, void (*iterator)(int16_t));
extern void hs_object_destroy_by_name(int16_t object_name_index);

void hs_object_destroy_containing(const char *name_string)
{
    hs_object_iterate_names_containing(name_string, hs_object_destroy_by_name);
}
