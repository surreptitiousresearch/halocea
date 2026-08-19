/* apply_dead_zone_and_rescale @ 0x836FB618 — apply a symmetric dead zone to an
 * analog axis value and linearly rescale the remaining range to full 16-bit span.
 * Within [-dead_range, dead_range] the result is 0; above/below it ramps to the
 * positive (+32767) / negative (-32768) extremes. */

float apply_dead_zone_and_rescale(float value, float dead_range)
{
    float result = 0.0f;

    if ( value > dead_range )
        result = ((value - dead_range) * 32767.0f) / (32767.0f - dead_range);

    if ( value < -dead_range )
        result = ((value + dead_range) * -32768.0f) / (dead_range - 32768.0f);

    return result;
}
