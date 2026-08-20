/* scripted_camera_set_animation @0x83704DD8 — drive the scripted camera from a cinematic animation. Looks up
 * the named animation in the given animation-graph tag (only valid for graph type 1; animations are 180-byte
 * records with the name at offset 0) and, if found, switches the camera to animation mode for the animation's
 * duration (animation.frame_count / 30).
 *
 * DEVIATION: the two indices were transcribed swapped. The binary stores the graph index into
 * animation_graph_index (stw r26, +0x38 @0x83704EA0, r26 = the r3 argument @0x83704DE4) and -1 into
 * relative_object_index (stw r8, +0x34 @0x83704EA4, li r8, -1 @0x83704E80) — two stw, not one 64-bit
 * store; corrected to match. */

#include <stdint.h>
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
        found = (int16_t)(found + 1);
        if ( found >= graph[29] )
            return;
    }

    camera_script_globals.field_of_view = 1.2217305f;
    camera_script_globals.mode = _camera_script_mode_animation;
    camera_script_globals.first_update = 1;
    camera_script_globals.animation_index = found;
    camera_script_globals.camera_point_index = -1;
    camera_script_globals.relative_object_index = -1;
    camera_script_globals.animation_graph_index = animation_graph_index;
    camera_script_globals.timer = (float)(anim->frame_count / 30);
}
