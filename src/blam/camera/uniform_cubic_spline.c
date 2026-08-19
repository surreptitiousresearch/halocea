/* uniform_cubic_spline @0x837C1880 — DEVIATION: the decompiler produced an unreadable deeply-nested cast
 * chain. Hand-traced disasm_range(0x837C1880,0x837C18E4) register-by-register (f1..f7 map to the 7 float
 * params f0,f1,f2,f3,t0,h,t in order) and confirmed this is Newton's forward-difference cubic
 * interpolation through 4 equally-spaced samples (t0, t0+h, t0+2h, t0+3h); rewritten in that clearer
 * nested form, verified term-for-term against the instruction sequence. */

float uniform_cubic_spline(float f0, float f1, float f2, float f3, float t0, float h, float t)
{
    float d0 = f1 - f0;
    float d1 = f2 - f1;
    float d2 = f3 - f2;

    float dd0 = d1 - d0;
    float ddd = (d2 - d1) - dd0;

    float inner = dd0 + (t - (t0 + 2.0f * h)) * ddd / (3.0f * h);
    float mid = d0 + (t - (t0 + h)) * inner / (2.0f * h);

    return f0 + (t - t0) / h * mid;
}
