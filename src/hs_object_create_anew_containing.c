/* hs_object_create_anew_containing @0x837F8A10 — re-create (destroy then create) every scenario object whose
 * name contains the given substring. */

#include <stdint.h>

extern void hs_object_iterate_names_containing(const char *name_string, void (*callback)(int16_t));
extern void hs_object_create_anew(int16_t object_name_index);

void hs_object_create_anew_containing(const char *name_string)
{
    hs_object_iterate_names_containing(name_string, hs_object_create_anew);
}
