/* refract_vector3d @0x836FCC58 — Snell's-law vector refraction: bends `incident` across `normal` by
 * `coefficient_of_refraction` (the ratio of refractive indices), writing the result (rescaled back to
 * incident's own original length, not unit length) into `refraction`. May produce NaN components under
 * total internal reflection (the intermediate cos_t = sqrt(1 - sin_t^2) is computed unguarded, matching
 * the compiled code).
 *
 * DEVIATION: the decompiler shows a phantom 4th parameter (`refraction`, never referenced in the body) and
 * a genuine 5th parameter `a5` that is actually the DB's real (and only) output parameter, `refraction`.
 * The compiled code also recomputes one large sub-expression three times identically (a decompiler/
 * compiler CSE artifact, not a behavioral difference); computed once here as `k`. */

#include "headers/real_vector3d.h"

extern float __fsqrts(float x);

real_vector3d *refract_vector3d(const real_vector3d *incident, const real_vector3d *normal,
        float coefficient_of_refraction, real_vector3d *refraction)
{
    float cross_x = normal->n[1] * incident->n[0] - normal->n[0] * incident->n[1];
    float cross_y = normal->n[2] * incident->n[1] - normal->n[1] * incident->n[2];
    float cross_z = normal->n[0] * incident->n[2] - normal->n[2] * incident->n[0];

    float incident_length = __fsqrts(incident->n[0] * incident->n[0] + incident->n[1] * incident->n[1]
                                    + incident->n[2] * incident->n[2]);
    float cross_magnitude = __fsqrts(cross_z * cross_z + cross_y * cross_y + cross_x * cross_x);

    float inverse_incident_length = 1.0f / incident_length;
    float sin_i = cross_magnitude * inverse_incident_length;
    float cos_i = (normal->n[2] * incident->n[2] + normal->n[0] * incident->n[0] + normal->n[1] * incident->n[1])
                * inverse_incident_length;

    float sin_t = sin_i * coefficient_of_refraction;
    float cos_t = __fsqrts(-(sin_t * sin_t - 1.0f));
    float k = (cos_t * sin_i + cos_i * sin_t) / sin_t;

    refraction->n[0] = normal->n[0] * k + incident->n[0];
    refraction->n[1] = normal->n[1] * k + incident->n[1];
    refraction->n[2] = normal->n[2] * k + incident->n[2];

    float refracted_length = __fsqrts(refraction->n[1] * refraction->n[1] + refraction->n[2] * refraction->n[2]
                                     + refraction->n[0] * refraction->n[0]);
    float rescale = incident_length / refracted_length;

    refraction->n[0] *= rescale;
    refraction->n[1] *= rescale;
    refraction->n[2] *= rescale;

    return refraction;
}
