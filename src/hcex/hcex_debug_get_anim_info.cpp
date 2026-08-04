/* hcex_debug_get_anim_info @0x836832C8 — HCEX debug helper: report an object's current animation frame count
 * and a short string describing which root-node movement channels the animation drives. Resolves the object
 * datum (object-header data-array), then the current animation record: animation graph index -> animations
 * block (180-byte elements), animation index. Writes the animation frame count to *frameCount, and selects
 * *info from the animation's frame-info type: 0 -> "none", 1 -> "xy", 2 -> "xyr", 3 -> "xyzr". A type > 3
 * leaves *info unchanged. */

#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/animation_graph.h"
#include "../headers/animation.h"
#include "../headers/global_tag_instances.h"
#include "../headers/animation_frame_info_type.h"

extern data_array *object_header_data;
extern char *_none, *_xy, *_xyr, *_xyzr;   /* frame-info channel name strings */

extern "C" void hcex_debug_get_anim_info(int objId, int *frameCount, char **info)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, objId)->datum;

    int animation_graph_index = object->object.animation.animation_graph_index;
    int animation_index = object->object.animation.state.index;

    animation_graph *graph_definition = *(animation_graph **)TAG_INSTANCE(animation_graph_index);
    animation *anim = &((animation *)graph_definition->animations.address)[animation_index];

    *frameCount = anim->frame_count;

    unsigned int frame_info_type = (unsigned __int16)anim->frame_info_type;
    if ( frame_info_type < NUMBER_OF_ANIMATION_FRAME_INFO_TYPES )
    {
        if ( frame_info_type == _animation_frame_info_xy_translation )
            *info = _xy;
        else if ( frame_info_type == _animation_frame_info_xy_translation_yaw_rotation )
            *info = _xyr;
        else if ( frame_info_type )   /* == _animation_frame_info_xyz_translation_yaw_rotation */
            *info = _xyzr;
        else
            *info = _none;
    }
}
