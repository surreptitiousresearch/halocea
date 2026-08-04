#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN_IFACE::GetBodyMatrNav(m3dMATR&) const @ 0x83193630
// Build the nav-space body local-coordinate matrix: origin at the nav body position, axes taken
// from the (normalized) body right / up / forward directions.
void aiBRAIN_IFACE::GetBodyMatrNav(m3dMATR &matr) const
{
    m3dV org   = GetBodyPosNav();
    m3dV up    = GetDirUp();
    m3dV right  = GetDirRight();
    m3dV dir   = GetDir();

    m3dNormalize(&up);
    m3dNormalize(&right);
    m3dNormalize(&dir);

    matr.MakeLCS2WCS(&org, &right, &up, &dir);
}
