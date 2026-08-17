/* compute_attenuation @0x836C5730 */
/* Returns a 1..0 attenuation factor: full (1.0) while `variable` <= maximum/2,
 * ramping linearly to 0 as `variable` approaches `maximum`, and 0 beyond it. */
float compute_attenuation(float variable, float maximum)
{
    if (variable < maximum)
    {
        if (variable > maximum * 0.5f)
            return (maximum - variable) / (maximum - maximum * 0.5f);
        return 1.0f;
    }
    return 0.0f;
}
