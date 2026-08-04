/* scalars_interpolate_and_clamp_0_to_1 — linearly interpolate a->b by `t`, then clamp to [0,1]
 * and store through `out`. Floats arrive in the FP registers (a=f1, b=f2, t=f3); the output pointer
 * is arg3 (r6).
 * DEVIATION (2026-07-31): four params, not seven. The three float args a/b/t each also consume their
 * GPR shadow slots (r3/r4/r5), so there are no separate r3/r4/r5 int params — the earlier `unused0..2`
 * were those float shadow slots misread as arguments. Matches the DB prototype and caller extern. */
void scalars_interpolate_and_clamp_0_to_1(float a, float b, float t, float *out)
{
    float value = ((1.0f - t) * a) + (b * t);

    if (value >= 0.0f)
    {
        if (value <= 1.0f)
            *out = ((1.0f - t) * a) + (b * t);
        else
            *out = 1.0f;
    }
    else
    {
        *out = 0.0f;
    }
}
