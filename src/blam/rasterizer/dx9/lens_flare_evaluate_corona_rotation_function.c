/* lens_flare_evaluate_corona_rotation_function @0x83791F48 — compute a corona's rotation angle (in turns, i.e.
 * radians/2pi) for a lens flare, selected by the corona's rotation-function id. The flare's compressed world
 * direction is decompressed to `forward`, then two scalars (a "sin" and "cos" component) are formed depending on
 * the function:
 *   0            : no rotation (returns 0).
 *   1 (view)     : angle of `forward` projected against the camera basis (the reassignment forward=camera.forward
 *                  is a dead store the compiler emitted; the dot products already read camera.forward directly).
 *   2 (world-up) : angle of -forward projected onto view_to_world rows 0 and 2.
 *   3 (flare-dir): angle built from forward crossed against view_to_world row 0, projected onto the flare->camera
 *                  offset.
 *   default (4)  : angle of the flare->camera offset projected onto view_to_world rows 0 and 2.
 * The final angle is atan2(sin_component, cos_component) * (1/2pi); if the function is 0 or the sin component is
 * exactly 0, the result is 0.
 *
 * DEVIATION: Hex-Rays modeled atan2 as taking two long-double register pairs (v13, v7) and invented a dead `v7`
 * local from the intermediate partial sums. Disasm at 0x837921AC-B4 shows the real call is atan2(f31, f30) =
 * atan2(sin_component, cos_component); v7 is dropped. 0x3E22F983 = 0.15915494f = 1/(2pi). The per-step (float)
 * casts are preserved for bit-exact single-precision rounding. */

#include <stdint.h>
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/real_vector3d.h"
#include "headers/lens_flare_corona_rotation_function.h"
#include <math.h>
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i);

float lens_flare_evaluate_corona_rotation_function(int16_t function, const rasterizer_lens_flare_submit_parameters *lens_flare_parameters)
{
    double cos_component = 1.0;
    double sin_component = 0.0;

    real_vector3d forward;
    real_vector3d *direction = uncompress_int32_to_real_vector3d(&forward,
            lens_flare_parameters->compressed_direction);
    forward.n[0] = direction->n[0];
    forward.n[1] = direction->n[1];
    forward.n[2] = direction->n[2];

    if ( function >= _lens_flare_corona_rotation_function_eye_in_light_space
      && function <= _lens_flare_corona_rotation_function_eye_to_light_in_eye_space )
    {
        switch ( function )
        {
            case _lens_flare_corona_rotation_function_light_in_eye_space:
            {
                float neg_x = -forward.n[0];
                float neg_y = -forward.n[1];
                float neg_z = -forward.n[2];
                sin_component = ((global_window_parameters.frustum.view_to_world.n[0][0] * neg_x)
                           + ((global_window_parameters.frustum.view_to_world.n[0][1] * neg_y)
                                   + (global_window_parameters.frustum.view_to_world.n[0][2] * neg_z)));
                cos_component = -((global_window_parameters.frustum.view_to_world.n[2][0] * neg_x)
                                   + ((global_window_parameters.frustum.view_to_world.n[2][1] * neg_y)
                                           + (global_window_parameters.frustum.view_to_world.n[2][2] * neg_z)));
                break;
            }
            case _lens_flare_corona_rotation_function_eye_to_light_in_light_space:
                cos_component = -((forward.n[0]
                                           * (lens_flare_parameters->position.n[0]
                                                   - global_window_parameters.camera.position.n[0]))
                                   + (((lens_flare_parameters->position.n[2]
                                                           - global_window_parameters.camera.position.n[2])
                                                   * forward.n[2])
                                           + ((lens_flare_parameters->position.n[1]
                                                           - global_window_parameters.camera.position.n[1])
                                                   * forward.n[1])));
                sin_component = ((((((global_window_parameters.frustum.view_to_world.n[0][0]
                                                                   * forward.n[2])
                                                           - (global_window_parameters.frustum.view_to_world.n[0][2]
                                                                   * forward.n[0]))
                                                   * forward.n[2])
                                           - (((global_window_parameters.frustum.view_to_world.n[0][1]
                                                                   * forward.n[0])
                                                           - (global_window_parameters.frustum.view_to_world.n[0][0]
                                                                   * forward.n[1]))
                                                   * forward.n[1]))
                                   * (lens_flare_parameters->position.n[0] - global_window_parameters.camera.position.n[0]))
                           + (((lens_flare_parameters->position.n[1]
                                                   - global_window_parameters.camera.position.n[1])
                                           * ((((global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[0])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][0]
                                                                           * forward.n[1]))
                                                           * forward.n[0])
                                                   - (forward.n[2]
                                                           * ((global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[1])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[2])))))
                                   + ((lens_flare_parameters->position.n[2]
                                                   - global_window_parameters.camera.position.n[2])
                                           * ((((global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[1])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[2]))
                                                           * forward.n[1])
                                                   - (((global_window_parameters.frustum.view_to_world.n[0][0]
                                                                           * forward.n[2])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[0]))
                                                           * forward.n[0])))));
                break;
            case _lens_flare_corona_rotation_function_eye_in_light_space:
                cos_component = -((forward.n[0] * global_window_parameters.camera.forward.n[0])
                                   + ((global_window_parameters.camera.forward.n[2] * forward.n[2])
                                           + (global_window_parameters.camera.forward.n[1] * forward.n[1])));
                sin_component = ((((((global_window_parameters.frustum.view_to_world.n[0][0]
                                                                   * forward.n[2])
                                                           - (global_window_parameters.frustum.view_to_world.n[0][2]
                                                                   * forward.n[0]))
                                                   * forward.n[2])
                                           - (((global_window_parameters.frustum.view_to_world.n[0][1]
                                                                   * forward.n[0])
                                                           - (global_window_parameters.frustum.view_to_world.n[0][0]
                                                                   * forward.n[1]))
                                                   * forward.n[1]))
                                   * global_window_parameters.camera.forward.n[0])
                           + ((global_window_parameters.camera.forward.n[1]
                                           * ((((global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[0])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][0]
                                                                           * forward.n[1]))
                                                           * forward.n[0])
                                                   - (forward.n[2]
                                                           * ((global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[1])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[2])))))
                                   + (global_window_parameters.camera.forward.n[2]
                                           * ((((global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[1])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][1]
                                                                           * forward.n[2]))
                                                           * forward.n[1])
                                                   - (((global_window_parameters.frustum.view_to_world.n[0][0]
                                                                           * forward.n[2])
                                                                   - (global_window_parameters.frustum.view_to_world.n[0][2]
                                                                           * forward.n[0]))
                                                           * forward.n[0])))));
                forward = global_window_parameters.camera.forward; /* dead store emitted by compiler */
                break;
            default:
                sin_component = ((global_window_parameters.frustum.view_to_world.n[0][0]
                                   * (lens_flare_parameters->position.n[0] - global_window_parameters.camera.position.n[0]))
                           + ((global_window_parameters.frustum.view_to_world.n[0][1]
                                           * (lens_flare_parameters->position.n[1]
                                                   - global_window_parameters.camera.position.n[1]))
                                   + (global_window_parameters.frustum.view_to_world.n[0][2]
                                           * (lens_flare_parameters->position.n[2]
                                                   - global_window_parameters.camera.position.n[2]))));
                cos_component = -((global_window_parameters.frustum.view_to_world.n[2][0]
                                           * (lens_flare_parameters->position.n[0]
                                                   - global_window_parameters.camera.position.n[0]))
                                   + ((global_window_parameters.frustum.view_to_world.n[2][1]
                                                   * (lens_flare_parameters->position.n[1]
                                                           - global_window_parameters.camera.position.n[1]))
                                           + (global_window_parameters.frustum.view_to_world.n[2][2]
                                                   * (lens_flare_parameters->position.n[2]
                                                           - global_window_parameters.camera.position.n[2]))));
                break;
        }
    }

    if ( !function || sin_component == 0.0 )
        return 0.0f;

    return ((float)atan2(sin_component, cos_component) * INV_TWO_PI);
}
