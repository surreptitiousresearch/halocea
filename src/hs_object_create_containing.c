/* hs_object_create_containing @0x837F8038 — create every scenario object whose name contains the given
 * substring. */

#include <stdint.h>

extern void hs_object_iterate_names_containing(const char *name_string, void (*callback)(__int16));
extern void hs_object_create(int16_t object_name_index);

void hs_object_create_containing(const char *name_string)
{
    hs_object_iterate_names_containing(name_string, hs_object_create);
}
