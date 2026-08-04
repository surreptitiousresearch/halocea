/* compute_composite_attenuation @0x836C5794 — combined angle/distance falloff factor in [0,1]. Each axis
 * is full strength (1.0) up to half its maximum, falls off linearly from half-max to max, and is zero at
 * or beyond max. The result is the product of the two factors (so a target beyond the maximum distance
 * yields 0 regardless of angle).
 *
 * DEVIATION: the DB prototype types the args as float; the decompiler shows them as double (the FPR-passed
 * floats). The fused-pair return temp is just the computed product. */

float compute_composite_attenuation(float angle, float maximum_angle, float distance, float maximum_distance)
{
    float angle_factor;
    if (angle < maximum_angle)
    {
        if (angle > maximum_angle * 0.5f)
            angle_factor = (maximum_angle - angle) / (maximum_angle - maximum_angle * 0.5f);
        else
            angle_factor = 1.0f;
    }
    else
    {
        angle_factor = 0.0f;
    }

    float distance_factor = 1.0f;
    float result;
    if (distance < maximum_distance)
    {
        if (distance > maximum_distance * 0.5f)
            distance_factor = (maximum_distance - distance) / (maximum_distance - maximum_distance * 0.5f);
        result = distance_factor * angle_factor;
    }
    else
    {
        result = 0.0f * angle_factor;
    }

    return result;
}
