#include <stdint.h>
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"
#include "headers/real_matrix4x3.h"

extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint8_t render_camera_view_to_screen(const render_camera *camera, const render_frustum *frustum, const real_point3d *view_point, real_point2d *screen_point);

unsigned __int8 render_camera_world_to_screen(const render_camera *camera, const render_frustum *frustum,
                                               const real_point3d *world_point, real_point2d *screen_point)
{
    real_point3d view_point;

    matrix4x3_transform_point(&frustum->world_to_view, world_point, &view_point);
    return render_camera_view_to_screen(camera, frustum, &view_point, screen_point);
}
