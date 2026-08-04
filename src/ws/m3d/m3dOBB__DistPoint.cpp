#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3dV.h"

// 0x82A70DFC  ?DistPoint@m3dOBB@@QAAHPBUm3dV@@MPAU2@PAMPAH@Z
// Closest-point / range test of `point` against this OBB.
//   vClosest (if non-null) receives the closest point in WORLD space,
//   dist      receives the distance, faceNmb the face index.
// Implemented by transforming the query into the box's local frame and delegating to
// m3dBOX::DistPoint, then transforming the closest point back out.
int m3dOBB::DistPoint(const m3dV *point, float distMax, m3dV *vClosest, float *dist, int *faceNmb)
{
    m3dMATR matrL2W_storage;
    m3dMATR *pMatrL2W = nullptr;
    // The inverse (local-to-world) matrix is only needed to map the closest point back to world,
    // so it is computed only when the caller wants that point.
    if ( vClosest )
        pMatrL2W = &matrL2W_storage;

    m3dMATR matrW2L;
    m3dBOX  bbLCS;
    this->GetMatrW2L(&matrW2L, pMatrL2W, &bbLCS);

    m3dV pointLCS;
    matrW2L.TransformPoint(point, &pointLCS);

    int result = bbLCS.DistPoint(&pointLCS, distMax, vClosest, dist, faceNmb);
    if ( result && vClosest )
        pMatrL2W->TransformPoint(vClosest); // map the local closest point back to world space
    return result;
}
