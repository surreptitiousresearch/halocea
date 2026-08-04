/* limit_pitch @0x83716658 — clamp desired_pitch's change from old_pitch to a multiplicative bend factor
 * (bend applied as a multiply when increasing, a divide when decreasing). No-op if maximum_bend is 0 or
 * desired_pitch already equals old_pitch.
 *
 * DEVIATION: the decompiler renders all three params as `double` and the return via a `*((float*)&x + 1)`
 * bit-pun; DB prototype and disasm confirm plain `float` params/return, reproduced here directly. */

float limit_pitch(float desired_pitch, float old_pitch, float maximum_bend)
{
    if (maximum_bend == 0.0f || desired_pitch == old_pitch)
        return desired_pitch;

    if (desired_pitch > old_pitch)
    {
        float limit = old_pitch * maximum_bend;
        return (desired_pitch > limit) ? limit : desired_pitch;
    }
    else
    {
        float limit = old_pitch / maximum_bend;
        return (desired_pitch > limit) ? desired_pitch : limit;
    }
}
