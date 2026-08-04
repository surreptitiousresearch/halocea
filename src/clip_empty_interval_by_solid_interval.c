BOOL clip_empty_interval_by_solid_interval(float *empty_t0, float *empty_t1, float solid_t0, float solid_t1)
{
    if ( *empty_t0 > solid_t1 )
        solid_t1 = *empty_t0;
    if ( solid_t0 > *empty_t1 )
        solid_t0 = *empty_t1;

    if ( *empty_t1 - solid_t1 <= solid_t0 - *empty_t0 )
        *empty_t1 = solid_t0;
    else
        *empty_t0 = solid_t1;

    return *empty_t0 > *empty_t1;
}
