/* nonuniform_cubic_spline @0x8380B288 — DEVIATION: the decompiler rendered this as an unreadable chain of
 * nested float-in-double casts around an undeclared `v9`. Hand-traced disasm_range(0x8380B288,0x8380B2E8)
 * register-by-register (f1..f9 = f0,f1,f2,f3,t0,t1,t2,t3,t per the fastcall FP convention) and confirmed
 * this is a standard Newton's-divided-differences cubic interpolation through the four (t_i, f_i) points,
 * evaluated at t. Rewritten in that clearer form; verified term-for-term against the fmadds chain. */

float nonuniform_cubic_spline(float f0, float f1, float f2, float f3,
        float t0, float t1, float t2, float t3, float t)
{
    float d10 = (f1 - f0) / (t1 - t0);
    float d21 = (f2 - f1) / (t2 - t1);
    float d32 = (f3 - f2) / (t3 - t2);

    float dd20 = (d21 - d10) / (t2 - t0);
    float dd31 = (d32 - d21) / (t3 - t1);

    float ddd30 = (dd31 - dd20) / (t3 - t0);

    return f0 + (t - t0) * (d10 + (t - t1) * (dd20 + (t - t2) * ddd30));
}
