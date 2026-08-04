/* light_new @0x836F92A8 — attach a dynamic light to an object. Only proceeds when the light definition
 * is either flagged on (flag 0x1) or has a lens flare tag (lens_flare.reference.index != -1). Allocates a light datum
 * (124-byte stride), records definition/object/attachment/function/change-color, mirrors the
 * definition's enable flag into the datum flags (bit 0x2 when on/animated), reconnects the light into
 * the map, stamps the lights marker, and registers it with the ws bridge. Returns the light datum
 * index, or -1. */

#include <stdint.h>
#include "headers/light_datum.h"
#include "headers/point_light_definition.h"
#include "headers/light_definition_flags.h"
#include "headers/point_light_flags.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/lights_globals.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void light_reconnect_to_map(uint16_t light_index);
extern void hcex_init_light(int light_index);

int light_new(int definition_index, int object_index, int16_t object_attachment_index,
              int16_t object_function_index, int16_t object_change_color_index)
{
    point_light_definition *light_definition = TAG_GET(point_light_definition, definition_index);
    if ( (light_definition->flags & (1u << _light_dynamic_bit)) == 0 && light_definition->lens_flare.reference.index == -1 )
        return -1;

    int light_index = datum_new(light_data);
    if ( light_index == -1 )
        return -1;

    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    light->flags = 0;
    light->definition_index = definition_index;
    light->object_index = object_index;
    light->attachment_marker_index = object_attachment_index;
    light->function_index = object_function_index;
    light->color_function_index = object_change_color_index;

    int enabled = light_definition->flags & (1u << _light_dynamic_bit);
    if ( enabled != 0 || light_definition->lens_flare.reference.index != -1 )
        light->flags = enabled | (1u << _point_light_connects_to_map_bit);
    else
        light->flags = enabled & ~(1u << _point_light_connects_to_map_bit); /* 0xFFFD */

    light->cluster_reference = -1;
    light->parent_light_index = -1;
    light_reconnect_to_map(light_index);
    light->marker = lights_globals.marker - 1;
    hcex_init_light(light_index);

    return light_index;
}
