/* sound_definition_get_maximum_distance @ 0x837F8A20 — a sound's max audible distance: the
 * definition's own value, or its sound class default when the definition leaves it at 0. */

#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/global_tag_instances.h"

float sound_definition_get_maximum_distance(int sound_definition_index)
{
    sound_definition *def = TAG_GET(sound_definition, sound_definition_index);
    float maximum_distance = def->maximum_distance;

    if ( maximum_distance == 0.0f )
        maximum_distance = sound_classes[def->class_index].maximum_distance;
    return maximum_distance;
}
