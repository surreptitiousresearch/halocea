/* ?createCapsuleConvexVerticesShape@hkpShapeCutterUtil@@SAPAVhkpConvexVerticesShape@@ABVhkVector4@@0MHH@Z @0x83F972F0 */
#include "../headers/havok/hkpShapeCutterUtil_boundary.h"

/* hkpShapeCutterUtil::createCapsuleConvexVerticesShape
   Build a capsule convex hull. The triangle mesh is produced by
   hkGeometryUtils::createCapsuleGeometry, then each triangle is turned into a
   connectivity face (with its plane equation) and a hkpConvexVerticesShape is
   built over the mesh vertices.

   DEVIATION: the original stages an identity transform through VMX128 and takes
   the geometry via an out-parameter; reconstructed here with an explicit
   out-pointer. The final freelist return of the temporary geometry is folded
   into hkGeometry_destruct (boundary). DEVIATION: a prior reconstruction lifted
   the staged transform into a phantom 6th parameter; the binary
   (?createCapsuleConvexVerticesShape@...@ABVhkVector4@@0MHH@Z, 0x83F972F0)
   takes exactly 5 args and builds the identity locally (vxor zero quads +
   stfs 1.0f on the rotation diagonal, 0x83F97334..0x83F9734C). */
hkpConvexVerticesShape *hkpShapeCutterUtil_createCapsuleConvexVerticesShape(const hkVector4 *top, const hkVector4 *bottom,
                                                                           float radius, int numSides, int numHeightSegments)
{
    hkTransform identity;
    hkGeometry *geometry = nullptr;
    hkArray<hkVector4> planeEquations;
    hkpConvexVerticesConnectivity *connectivity;
    hkpConvexVerticesShape *shape;
    int i;

    {
        int lane;
        for (lane = 0; lane < 4; ++lane)
        {
            identity.m_rotation.m_col0.m_quad.___u0.v[lane] = 0.0f;
            identity.m_rotation.m_col1.m_quad.___u0.v[lane] = 0.0f;
            identity.m_rotation.m_col2.m_quad.___u0.v[lane] = 0.0f;
            identity.m_translation.m_quad.___u0.v[lane] = 0.0f;
        }
        identity.m_rotation.m_col0.m_quad.___u0.v[0] = 1.0f;
        identity.m_rotation.m_col1.m_quad.___u0.v[1] = 1.0f;
        identity.m_rotation.m_col2.m_quad.___u0.v[2] = 1.0f;
    }
    hkGeometryUtils_createCapsuleGeometry(top, bottom, radius, numSides, numHeightSegments, &identity, &geometry);

    connectivity = hkpShapeCutterUtil_newConnectivity();
    planeEquations.m_data = nullptr;
    planeEquations.m_size = 0;
    planeEquations.m_capacityAndFlags = 0x80000000;

    if (geometry->m_triangles.m_size > 0)
        hkArrayUtil__reserve(&planeEquations.m_data, geometry->m_triangles.m_size, 16);

    for (i = 0; i < geometry->m_triangles.m_size; ++i)
    {
        hkGeometry_Triangle *tri = &geometry->m_triangles.m_data[i];
        int idx[3];
        idx[0] = tri->m_a;
        idx[1] = tri->m_b;
        idx[2] = tri->m_c;
        hkpShapeCutterUtil__addTriangle(idx, connectivity, &geometry->m_vertices, &planeEquations);
    }

    shape = hkpShapeCutterUtil_finishShape(&geometry->m_vertices, &planeEquations, connectivity);

    if (geometry)
        hkGeometry_destruct(geometry);

    return shape;
}
