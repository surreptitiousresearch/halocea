/* render_camera_hack_frustum_z @ 0x8376BA58
   Temporarily overrides the frustum's projection z mapping. (-1,-1) saves the
   current z coefficients; (0,0) restores them; any other pair installs a new
   near/far z projection (column 2 of the projection matrix). */

#include "headers/render_frustum.h"
#include "headers/blam_data_globals.h"


void render_camera_hack_frustum_z(render_frustum *frustum, float z_near, float z_far)
{
    if ( z_near == -1.0f && z_far == -1.0f )
    {
        previous_projection_coefficients[0] = frustum->projection_matrix[0][2];
        previous_projection_coefficients[1] = frustum->projection_matrix[1][2];
        previous_projection_coefficients[2] = frustum->projection_matrix[2][2];
        previous_projection_coefficients[3] = frustum->projection_matrix[3][2];
    }
    else if ( z_near == 0.0f && z_far == 0.0f )
    {
        frustum->projection_matrix[0][2] = previous_projection_coefficients[0];
        frustum->projection_matrix[1][2] = previous_projection_coefficients[1];
        frustum->projection_matrix[2][2] = previous_projection_coefficients[2];
        frustum->projection_matrix[3][2] = previous_projection_coefficients[3];
    }
    else
    {
        frustum->projection_matrix[0][2] = 0.0f;
        frustum->projection_matrix[1][2] = 0.0f;
        frustum->projection_matrix[2][2] = -((z_near + z_far) / (z_far - z_near));
        frustum->projection_matrix[3][2] = ((z_near * z_far) * -2.0f) / (z_far - z_near);
    }
}
