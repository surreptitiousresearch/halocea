/* hcex_prepare_locations @0x823CE820 — position the named marker nodes of an animINST from arrays of
 * Halo-space positions and direction vectors. For each named point it normalizes the name, finds the
 * matching node under the instance's object tree, builds a local->world transform from the (axis-converted,
 * feet-scaled) position and forward vector, stores it as the node's matrLT, and recomputes matrModel as
 * matrLT * parent^-1.
 *
 * Halo->ws axis conversion: (x, y, z)_halo -> (x, z, -y) and the position is scaled by 3.0480001
 * (Halo world units -> ws units). The ws math/scene types are boundary types (see boundary header). */

#include <string.h>
#include "../headers/hcex/hcex_prepare_locations_boundary.h"

#define HALO_TO_WS_SCALE 3.0480001f

void hcex_prepare_locations(animINST *pInst, hcex_float3 *positions, hcex_float3 *vectors,
        const char **names, int npoints)
{
    int i;

    if ( !names || npoints <= 0 )
        return;

    for ( i = 0; i < npoints; ++i )
    {
        char node_name[64];
        objOBJ *node;

        strncpy(node_name, names[i], sizeof(node_name));
        node_name[63] = 0;
        hcex_conv_name(node_name, sizeof(node_name));
        if ( node_name[0] == 0 )
            strcpy(node_name, "normal");   /* deviation: original is a 7-byte unrolled copy from a data label */

        node = objFindName(animINST_get_pObj(pInst), node_name);
        if ( node )
        {
            m3dV origin;
            m3dV forward;
            m3dMATR local_to_world;
            m3dMATR parent_world;
            m3dMATR parent_inverse;

            parent_world = node->parent->matrLT;
            m3dMATR_Invert(&parent_world, &parent_inverse);

            origin.x = positions[i].x * HALO_TO_WS_SCALE;
            origin.y = positions[i].z * HALO_TO_WS_SCALE;
            origin.z = positions[i].y * -HALO_TO_WS_SCALE;
            forward.x = vectors[i].x;
            forward.y = vectors[i].z;
            forward.z = -vectors[i].y;

            m3dMATR_MakeLCS2WCS_VZ(&local_to_world, &origin, &forward);
            node->matrLT = local_to_world;
            m3dMultiplyMatr(&local_to_world, &parent_inverse, &node->matrModel);
        }
    }
}
