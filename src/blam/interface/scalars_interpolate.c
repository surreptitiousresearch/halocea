/* scalars_interpolate @0x836FC9F0 — linear interpolation of a scalar: result = (1-t)*a + t*b. */

void scalars_interpolate(float a, float b, float t, float *result)
{
    *result = ((1.0f - t) * a) + (b * t);
}
