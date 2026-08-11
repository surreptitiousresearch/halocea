/* hcex_init_light @0x83681EE0 — when a newly-attached light sits on the object's "flashlight" marker,
 * register it with the ws bridge so it renders as the player flashlight. Reads the light datum
 * (124-byte stride), verifies the owning object, resolves the attachment's marker name, and if it
 * equals "flashlight" calls hcex_create_light with the light's tag name. */

#include <stdint.h>
#include "../headers/light_datum.h"
#include "../headers/data_array.h"
#include "../headers/object_type.h"

extern "C" data_array *light_data;

extern "C" void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern "C" const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern "C" char *tag_get_name(int tag_index);
extern "C" void        hcex_create_light(int lightId, int object_index, const char *name);

extern "C" int strcmp(const char *a, const char *b);

extern "C" void hcex_init_light(int lightId)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, lightId);
    if ( !object_try_and_get_and_verify_type(light->object_index, object_mask_all) )
        return;

    int attachment_index = (uint16_t)light->attachment_marker_index;   /* DEVIATION: the int16_t field sign-extended into the int local, so the `== 0xFFFF` guard below never matched and an unattached light queried marker -1; binary zero-extends, lhz r4,0x5C(r31) @0x83681F24 + cmplwi cr6,r4,0xFFFF @0x83681F28. hcex_obj_collect.cpp:44 already carries this cast on the same field. */
    if ( attachment_index == 0xFFFF )
        return;

    const char *marker_name = object_get_attachment_marker_name(light->object_index, attachment_index);
    if ( !marker_name )
        return;

    if ( strcmp(marker_name, "flashlight") == 0 )
    {
        const char *name = tag_get_name(light->definition_index);
        hcex_create_light(lightId, light->object_index, name);
    }
}
