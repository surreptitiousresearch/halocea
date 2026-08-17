/* rasterizer_widget_submit_occlusion_test @0x836A63E8 */
/* rasterizer_widget_submit_occlusion_test 0x836A63E8 (thunk) — tail-call thunk to the underscore
 * implementation, which returns the number of occlusion-test pixels that passed.
 *
 * DEVIATION: the database types this entry as void; it returns int (the implementation's pixel count, used by
 * the lens-flare occlusion path). */
struct real_point3d;
#include "headers/real_point3d.h"
extern int _rasterizer_widget_submit_occlusion_test(const real_point3d *point, float radius, int index);

int rasterizer_widget_submit_occlusion_test(const struct real_point3d *point, float radius, int index)
{
    return _rasterizer_widget_submit_occlusion_test(point, radius, index);
}
