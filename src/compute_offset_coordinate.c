/* compute_offset_coordinate @0x83730A08 — computes a scrolling texture-coordinate offset in [0,1):
 * elapsed time in ticks * rate (units/sec) * 0.001, wrapped into the unit interval via fmod.
 *
 * DEVIATION: the decompiler invented two extra parameters (a3, a4) and rendered the whole thing through
 * bogus __SPAIR64__/long-double casts. Disasm shows only 2 real params; the divisor passed to fmod is the
 * literal constant 1.0, not a parameter. */

extern double fmod(double x, double y);

float compute_offset_coordinate(int time, float delta_per_sec)
{
    return (float)fmod(((float)time * delta_per_sec * 0.001f), 1.0);
}
