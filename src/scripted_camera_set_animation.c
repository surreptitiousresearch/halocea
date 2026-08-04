/* scripted_camera_set_animation @0x83704DD8 — drive the scripted camera from a cinematic animation. Looks up
 * the named animation in the given animation-graph tag (only valid for graph type 1; animations are 180-byte
 * records with the name at offset 0) and, if found, switches the camera to animation mode for the animation's
 * duration (animation.frame_count / 30).
 *
 * DEVIATION: the binary writes relative_object_index/animation_graph_index as one 64-bit store, placing the
 * graph index in relative_object_index and -1 in animation_graph_index; reproduced as written. */

#include "headers/camera_script_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/animation.h"
#include "headers/camera_script_mode.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);

void scripted_camera_set_animation(unsigned int animation_graph_index, const char *animation_name)
{
    if ( animation_graph_index == -1 )
        return;

    int *graph = TAG_GET(int, animation_graph_index);
    if ( graph[26] != 1 )
        return;

    int animation_count = graph[29];
    if ( animation_count <= 0 )
        return;

    int found = 0;
    animation *anim = 0;
    while ( 1 )
    {
        anim = &((animation *)graph[30])[found];
        if ( !stricmp(animation_name, anim->name) )
            break;
        found = (__int16)(found + 1);
        if ( found >= graph[29] )
            return;
    }

    camera_script_globals.field_of_view = 1.2217305f;
    camera_script_globals.mode = _camera_script_mode_animation;
    camera_script_globals.first_update = 1;
    camera_script_globals.animation_index = found;
    camera_script_globals.camera_point_index = -1;
    camera_script_globals.relative_object_index = animation_graph_index;
    camera_script_globals.animation_graph_index = -1;
    camera_script_globals.timer = (float)(anim->frame_count / 30);
}
