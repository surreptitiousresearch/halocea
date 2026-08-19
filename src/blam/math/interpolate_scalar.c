/* interpolate_scalar @0x836C6AC0 — move *current toward `desired`, clamped so it changes by at most
 * `maximum_speed` in either direction this step. */

void interpolate_scalar(float *current, float desired, float maximum_speed)
{
    float delta = desired - *current;
    if ( delta >= -maximum_speed )
    {
        if ( delta <= maximum_speed )
            *current = *current + (desired - *current); /* reached target */
        else
            *current = *current + maximum_speed;
    }
    else
    {
        *current = *current + -maximum_speed;
    }
}
