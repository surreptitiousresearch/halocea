/* Square3x3 @0x837E4020 — square a symmetric 3x3 matrix stored row-major as float[3][3], writing only the
 * upper triangle of the (symmetric) result: m2 = m·m using m's symmetry (m[1][0]==m[0][1] etc.). No xrefs
 * in the database (dead Blam math, kept for completeness of the size tier). */

void Square3x3(float (*m)[3], float (*m2)[3])
{
    const float *a = &m[0][0];
    float *r = &m2[0][0];

    r[0] = a[2] * a[2] + a[0] * a[0] + a[1] * a[1];
    r[1] = (a[4] + a[0]) * a[1] + a[5] * a[2];
    r[2] = (a[8] + a[0]) * a[2] + a[5] * a[1];
    r[4] = a[1] * a[1] + a[5] * a[5] + a[4] * a[4];
    r[5] = (a[8] + a[4]) * a[5] + a[2] * a[1];
    r[8] = a[2] * a[2] + a[8] * a[8] + a[5] * a[5];
}
