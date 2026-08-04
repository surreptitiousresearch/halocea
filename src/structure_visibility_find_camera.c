/* structure_visibility_find_camera @0x837C46A0 — resolve which BSP leaf, cluster and sky the render camera is
 * in, storing the results in the render globals. The camera position is point-tested against the structure's
 * collision BSP; on a miss the previous leaf is reused unless it is now out of range. From the leaf the cluster
 * index is read, and from the cluster the visible sky index; if that sky has a model the visible_sky_model flag
 * is set. */

#include "headers/render_globals.h"
#include "headers/render_camera.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/structure_cluster.h"
#include "headers/bsp3d.h"
#include "headers/sky.h"
#include "headers/blam_data_globals.h"


extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);
extern sky *scenario_get_sky(__int16 sky_index);

void structure_visibility_find_camera(const render_camera *camera)
{
    structure_bsp *structure = global_structure_bsp;
    int leaf_index = bsp3d_test_point((const bsp3d *)structure->collision_bsp.address, 0, &camera->position);

    if (leaf_index == -1)
    {
        leaf_index = render.leaf_index;
        if (render.leaf_index >= structure->leaves.count)
            leaf_index = -1;
    }
    render.leaf_index = leaf_index;

    render.cluster_index = -1;
    render.visible_sky_index = -1;
    render.visible_sky_model = 0;

    if (leaf_index != -1)
    {
        render.cluster_index = ((structure_leaf *)structure->leaves.address)[leaf_index].cluster_index;
        render.visible_sky_index = ((structure_cluster *)structure->clusters.address)[render.cluster_index].sky_index;
        sky *visible_sky = scenario_get_sky(render.visible_sky_index);
        if (visible_sky && visible_sky->model.index != -1)
            render.visible_sky_model = 1;
    }
}
