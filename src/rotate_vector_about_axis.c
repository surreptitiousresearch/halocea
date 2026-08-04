/* rotate_vector_about_axis @ 0x836FC670 — rotate vector v about unit axis n by an angle given as its
 * (sine, cosine), in place, via Rodrigues' rotation formula. The cross-product terms and the
 * axis-projection (dot * (1 - cos)) are cached before any component of v is overwritten, since each
 * output component reads the original v.
 * Attested: sine/cosine arrive in f1/f2 and feed fmuls/fnmsubs directly (no frsp) => float params;
 * no r3 write + 0/12 sampled callers consume r3 => void return (decompiler's "returns v" was r3
 * still holding the argument). All former (float) casts were double-local artifacts, not frsp. */

#include "headers/real_vector3d.h"

void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine)
{
    float orig_y   = v->n[1];
    float z_cos    = v->n[2] * cosine;
    float cross_xy = v->n[0] * n->n[1] - n->n[0] * v->n[1];
    float cross_zx = n->n[0] * v->n[2] - v->n[0] * n->n[2];
    float dot_omc  = (n->n[2] * v->n[2] + (n->n[0] * v->n[0] + n->n[1] * v->n[1]))
                   * (1.0f - cosine);

    v->n[0] = -((v->n[1] * n->n[2] - n->n[1] * v->n[2]) * sine
              - (n->n[0] * dot_omc + v->n[0] * cosine));
    v->n[1] = -(cross_zx * sine - (orig_y * cosine + dot_omc * n->n[1]));
    v->n[2] = -(cross_xy * sine - (n->n[2] * dot_omc + z_cos));
}
