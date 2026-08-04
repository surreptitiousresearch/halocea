/* hcex_make_inst_matr @0x823D0D78 — convert an hcex_matr4x3 (Blam/Halo basis) into a ws m3dMATR instance
 * transform. Applies the Halo->ws axis conversion (x, y, z)_halo -> (x, z, -y) to the position and to each
 * basis vector, scales the position by 3.0480001 (Halo world units -> ws units), validates and negates the
 * left axis (handedness flip), builds the local->world matrix, then offsets it into world space by
 * hcexWorldOffset. The ws math types are boundary types. */

#include "../headers/hcex/hcex_prepare_locations_boundary.h"   /* m3dV, m3dMATR */
#include "../headers/hcex/hcex_matr4x3.h"                      /* hcex_matr4x3, hcex_float3 */

#define HALO_TO_WS_SCALE 3.0480001f

extern const m3dV hcexWorldOffset;
extern void _m3dCheckValid(m3dV *v);
extern void m3dMATR_MakeLCS2WCS(m3dMATR *out, const m3dV *origin,
        const m3dV *forward, const m3dV *up, const m3dV *left);
extern void m3dMATR_Translate(m3dMATR *m, const m3dV *offset, int mode);

void hcex_make_inst_matr(const hcex_matr4x3 *in, m3dMATR *out)
{
    m3dV origin;
    m3dV forward;
    m3dV up;
    m3dV left;

    origin.x = in->pos.x * HALO_TO_WS_SCALE;
    origin.y = in->pos.z * HALO_TO_WS_SCALE;
    origin.z = in->pos.y * -HALO_TO_WS_SCALE;

    forward.x = in->forward.x;
    forward.y = in->forward.z;
    forward.z = -in->forward.y;

    up.x = in->up.x;
    up.y = in->up.z;
    up.z = -in->up.y;

    left.x = in->left.x;
    left.y = in->left.z;
    left.z = -in->left.y;

    _m3dCheckValid(&left);
    left.x = -left.x;
    left.y = -left.y;
    left.z = -left.z;

    m3dMATR_MakeLCS2WCS(out, &origin, &forward, &up, &left);
    m3dMATR_Translate(out, &hcexWorldOffset, 2);
}
