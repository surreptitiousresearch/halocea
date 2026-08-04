#include "objOBJ.h"
#include "../ps/psSECTION.h"
// 0x82660760  ?_UnshareAll@objOBJ@@QAAHXZ
// Give this object private copies of every shared resource: reset its ps section, then unshare the
// name, geometry, vertex/face lists, all texcoord + tangent channels, normals, color channels, the
// face split, bone lists, instanced and interleaved data. Finally drop the geometry-sharing host.
// Returns 1 on success, 0 if any unshare step fails.

int objOBJ::_UnshareAll()
{
    psSECTION defaultSection;   // default-construct an empty section...
    this->ps = defaultSection;  // ...and reset this object's ps to it

    if (!_UnshareName())
        return 0;

    if (!this->pGeom)
        return 1;

    _UnshareGeom();
    if (!_UnshareVertList() || !_UnshareFaceList())
        return 0;

    for (int tcInd = 0; tcInd < 5; ++tcInd) {
        if (!_UnshareTexCoord(tcInd) || !_UnshareVertTangentList(tcInd))
            return 0;
    }

    if (!_UnshareVertNormalList())
        return 0;

    for (int vcInd = 0; vcInd < 3; ++vcInd) {
        if (!_UnshareColorList(vcInd))
            return 0;
    }

    if (!_UnshareFaceSplit())
        return 0;

    _UnshareVertBoneList();
    _UnshareInstancedData();
    _UnshareInterleavedData();
    SetGeomSharingHost(nullptr);
    SetGeomSharingOfsVtx(0);
    return 1;
}
