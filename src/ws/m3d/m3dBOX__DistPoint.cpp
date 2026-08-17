/* ?DistPoint@m3dBOX@@QAAHPBUm3dV@@MPAU2@PAMPAH@Z @0x8265BFD8 */
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include "../../headers/ppc_intrinsics.h"

// 0x8265C008  ?DistPoint@m3dBOX@@QAAHPBUm3dV@@MPAU2@PAMPAH@Z
// Closest point / range test of `p` against this axis-aligned box.
//   returns 2 when p is inside, 1 when outside but within `distMax`, 0 when out of range.
//   vClosest (m3dV*)  -> closest point on/in the box
//   dist     (float*) -> distance from p to that point
//   faceNmb  (int*)   -> index of the crossed face (0 inside, 1..5 per face)
// NOTE: the decompiler mislabels the three output params (float distMax consumes the r5 GPR
// slot, so the pointers land in r6/r7/r8); names here follow the mangled signature + prologue.
int m3dBOX::DistPoint(const m3dV *p, float distMax, m3dV *vClosest, float *dist, int *faceNmb)
{
    int   faceIdx = 0;
    if ( faceNmb )
        *faceNmb = 0;

    if ( this->IsBelongPoint(p) )
    {
        if ( vClosest )
        {
            vClosest->x = p->x;
            vClosest->y = p->y;
            vClosest->z = p->z;
        }
        if ( dist )
            *dist = 0.0f;
        return 2;
    }

    m3dV  closest;
    float planarDist; // distance component in the XZ plane

    float px = p->x;
    if ( px <= this->fur.x )
    {
        if ( px >= this->bll.x )
        {
            float pz = p->z;
            if ( pz <= this->fur.z )
            {
                if ( pz >= this->bll.z )
                {
                    closest.z = p->z;
                    planarDist = 0.0f;
                }
                else
                {
                    closest.z = this->bll.z;
                    planarDist = this->bll.z - pz;
                }
            }
            else
            {
                closest.z = this->fur.z;
                faceIdx = 2;
                planarDist = pz - this->fur.z;
            }
            closest.x = px;
        }
        else
        {
            float pz = p->z;
            if ( pz >= this->bll.z )
            {
                closest.x = this->bll.x;
                if ( pz <= this->fur.z )
                {
                    closest.z = pz;
                    planarDist = this->bll.x - px;
                }
                else
                {
                    closest.z = this->fur.z;
                    planarDist = m3xzDist(p, &closest);
                }
                faceIdx = 3;
            }
            else
            {
                planarDist = m3xzDist(p, &this->bll);
                faceIdx = 3;
                closest.y = this->bll.y;
                closest.x = this->bll.x;
                closest.z = this->bll.z;
            }
        }
    }
    else
    {
        float pz = p->z;
        if ( pz <= this->fur.z )
        {
            if ( pz >= this->bll.z )
            {
                faceIdx = 1;
                planarDist = p->x - this->fur.x;
                closest.x = this->fur.x;
                closest.z = pz;
            }
            else
            {
                closest.x = this->fur.x;
                closest.z = this->bll.z;
                planarDist = m3xzDist(p, &closest);
                faceIdx = 1;
            }
        }
        else
        {
            planarDist = m3xzDist(p, &this->fur);
            faceIdx = 1;
            closest.y = this->fur.y;
            closest.x = this->fur.x;
            closest.z = this->fur.z;
        }
    }

    float yFace;
    float y = p->y;
    if ( y >= this->bll.y )
    {
        if ( y <= this->fur.y )
        {
            closest.y = p->y;
            if ( planarDist > distMax )
                return 0;
            if ( dist )
                *dist = planarDist;
            goto exit_found;
        }
        if ( (float)(p->y - this->fur.y) > distMax )
            return 0;
        closest.y = this->fur.y;
        faceIdx = 5;
        _m3dCheckValid(planarDist);
        yFace = this->fur.y;
    }
    else
    {
        if ( (float)(this->bll.y - p->y) > distMax )
            return 0;
        closest.y = this->bll.y;
        faceIdx = 4;
        _m3dCheckValid(planarDist);
        yFace = this->bll.y;
    }

    {
        float dy = yFace - p->y;
        _m3dCheckValid(dy);
        float distSq = (dy * dy) + (planarDist * planarDist);
        if ( distSq > (float)(distMax * distMax) )
            return 0;
        if ( dist )
        {
            _m3dCheckValid(distSq);
            *dist = __fsqrts(distSq); // sqrtf of the squared distance
        }
    }

exit_found:
    if ( vClosest )
    {
        vClosest->x = closest.x;
        vClosest->y = closest.y;
        vClosest->z = closest.z;
    }
    if ( faceNmb )
        *faceNmb = faceIdx;
    return 1;
}
