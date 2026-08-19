/* sound_definition_get_minimum_distance @ 0x837F8A68 — a sound's minimum (full-volume) distance:
 * the definition's own value, or its sound class default when left at 0. */

#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/global_tag_instances.h"

float sound_definition_get_minimum_distance(int sound_definition_index)
{
    sound_definition *def = TAG_GET(sound_definition, sound_definition_index);
    float minimum_distance = def->minimum_distance;

    if ( minimum_distance == 0.0f )
        minimum_distance = sound_classes[def->class_index].minimum_distance;
    return minimum_distance;
}
