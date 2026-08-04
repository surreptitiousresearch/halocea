/* camera_track_splut @ 0x837C2810 — sample a unit/seat's camera-track animation at a given pitch to get
 * the third-person eye offset. The seat's camera definition names a camera-track animation (falling back
 * to the game globals' default camera animation); the pitch (mapped from [-pi/2, pi/2] to [0,1]) selects
 * a keyframe span, and a uniform cubic spline through four consecutive keyframe vectors produces the
 * interpolated offset.
 *
 * The animation tag (camera_track_definition) holds its keyframes in the control_points tag_block: count =
 * frame count, address = keyframe array (camera_track_control_point, 60 bytes, position vector at +0). The
 * decompiler labels an extra phantom a4 output; the disassembly shows the real output is the 3rd argument
 * (`offset`), which is what uniform_cubic_spline_vector3d writes. */

#include "headers/unit_camera.h"
#include "headers/unit_camera_track.h"
#include "headers/camera_track_definition.h"
#include "headers/camera_track_control_point.h"
#include "headers/game_globals_camera.h"
#include "headers/real_vector3d.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"

extern void uniform_cubic_spline_vector3d(real_vector3d *result, const real_vector3d *f0, const real_vector3d *f1, const real_vector3d *f2, const real_vector3d *f3, float t0, float h, float t);

void camera_track_splut(const unit_camera *camera, float pitch, real_vector3d *offset)
{
    int track_count = camera->unit_camera_tracks.count;
    int animation_index;
    camera_track_definition *animation_tag;
    int frame_count;
    camera_track_control_point *keyframes;
    double inv_span;
    double fraction;
    int frame;
    int clamped;

    /* select the camera-track animation; fall back to the global default when missing/unset */
    if ( !track_count )
    {
        animation_index = ((game_globals_camera *)global_game_globals->camera.address)->default_unit_camera_track.index;
    }
    else
    {
        /* sign-folded track selection: index = 1 & (count - 1) (per disasm) */
        unit_camera_track *track = &((unit_camera_track *)camera->unit_camera_tracks.address)[1 & (track_count - 1)];
        animation_index = track->track.index;
        if ( animation_index == -1 )
            animation_index = ((game_globals_camera *)global_game_globals->camera.address)->default_unit_camera_track.index;
    }

    animation_tag = TAG_GET(camera_track_definition, animation_index);
    frame_count = animation_tag->control_points.count;
    keyframes   = (camera_track_control_point *)animation_tag->control_points.address;

    inv_span = (1.0f / (float)(frame_count - 1));
    fraction = ((pitch + 1.5707964f) * 0.31830987f);   /* (pitch+pi/2)/pi */
    frame = (__int16)(int)((float)(frame_count - 1) * (float)fraction);

    /* back the start frame off until a 4-keyframe span fits and we don't exceed the target frame */
    clamped = frame;
    if ( frame > 0 )
    {
        do
        {
            if ( clamped + 4 <= frame_count && clamped <= frame - 1 )
                break;
            clamped = (__int16)(clamped - 1);
        }
        while ( clamped > 0 );
    }

    uniform_cubic_spline_vector3d(
        offset,
        &keyframes[clamped].position,
        &keyframes[clamped + 1].position,
        &keyframes[clamped + 2].position,
        &keyframes[clamped + 3].position,
        ((float)clamped * (float)inv_span),
        (float)inv_span,
        (float)fraction);
}
