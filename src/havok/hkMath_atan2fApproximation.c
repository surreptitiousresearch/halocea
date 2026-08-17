/* hkMath_atan2fApproximation @0x838E24D8 */
/* Branch-selected cubic min-max approximation of atan2(y, x). DEVIATION: the
   decompiler models both args as double (PPC FP promotion); they are floats.
   eps = 2^-23 guards the divide when the dominant axis is ~0. The polynomial
   poly = 0.093522817*z^3 + 0.121079*z^2 - z is evaluated on the ratio of the
   smaller magnitude over the larger, then folded into the correct quadrant. */
extern double __fabs(double x);   /* same local extern the Blam-C TUs use (accelerate_to_position.c et al.) */

float hkMath_atan2fApproximation(float x, float y)
{
    const float eps = 0.00000011920929f;
    float ax = __fabs(x);
    float ay = __fabs(y);
    float angle;

    if (ax > ay)
    {
        float z = ay / (ax + eps);
        float poly = z * z * z * 0.093522817f + z * z * 0.121079f - z;
        angle = 1.5707964f + poly;
    }
    else
    {
        float z = ax / (ay + eps);
        float poly = z * z * z * 0.093522817f + z * z * 0.121079f - z;
        angle = -poly;
    }

    if (y < 0.0f)
        angle = 3.1415927f - angle;
    return (x >= 0.0f) ? angle : -angle;
}
