/* hcex_obj_get_anim_info @0x836838C0 — report the currently-playing animation of a live Blam object. Reads
 * the object's animation graph index and animation index; on a valid pair it resolves the animation_graph tag
 * definition (via the global tag instance table), indexes its animations block (180-byte animation elements)
 * and returns the current frame, the animation's frame count and a pointer to the animation element. Returns 0
 * when the object has no active animation. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/animation_graph.h"
#include "../headers/animation.h"
#include "../headers/global_tag_instances.h"

extern data_array *object_header_data;

extern "C" int hcex_obj_get_anim_info(int id, int *frameCur, int *frameCount, char **ptrName)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, id)->datum;

    int animation_graph_index = object->object.animation.animation_graph_index;
    if ( animation_graph_index == -1 )
        return 0;

    int animation_index = object->object.animation.state.index;
    if ( (__int16)animation_index == -1 )
        return 0;

    animation_graph *graph_definition = *(animation_graph **)TAG_INSTANCE(animation_graph_index);
    animation *anim = &((animation *)graph_definition->animations.address)[(__int16)animation_index];

    *frameCur   = object->object.animation.state.frame_index;
    *frameCount = anim->frame_count;
    *ptrName    = anim->name;
    return 1;
}
