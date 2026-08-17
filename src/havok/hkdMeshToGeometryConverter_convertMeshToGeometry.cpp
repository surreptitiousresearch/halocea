/* ?convertMeshToGeometry@hkdMeshToGeometryConverter@@SAPAVhkdGeometry@@ABUInput@1@PBVhkMeshShape@@PBD@Z @0x83872148 */
#include "../headers/havok/hkdMeshToGeometryConverter_boundary.h"

/* hkdMeshToGeometryConverter::convertMeshToGeometry — public entry point: build a
   transient Impl worker from the input options, convert the mesh shape into a
   fracture geometry, and tear the worker down. */
hkdGeometry *hkdMeshToGeometryConverter_convertMeshToGeometry(
    const hkdMeshToGeometryConverter_Input *input,
    const hkMeshShape *shape,
    const char *meshName)
{
    hkdMeshToGeometryConverterImpl impl;
    hkdGeometry *geometry;

    hkdMeshToGeometryConverterImpl_construct(&impl, input);
    geometry = hkdMeshToGeometryConverterImpl_convertShapeToGeometry(&impl, shape, meshName);
    hkdMeshToGeometryConverterImpl_destruct(&impl);
    return geometry;
}
