/* sound_scale_value @0x837154A8 */
/* Params are Blam `real` (float); the decompiler defaulted them to `double` and wrapped the return in a
 * double→float idiom — the binary computes entirely in single precision (fsubs/fmadds/fmuls, no frsp).
 *   result = (upper_bound_modifier - lower_bound_modifier) * scale + lower_bound_modifier, then * base. */
float sound_scale_value(float base, float lower_bound_modifier, float upper_bound_modifier, float scale)
{
    return ((upper_bound_modifier - lower_bound_modifier) * scale + lower_bound_modifier) * base;
}
