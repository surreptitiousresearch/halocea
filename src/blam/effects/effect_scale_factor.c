/* effect_scale_factor @0x83737F38 — lerps from zero_scale_factor to scale (confirmed via disasm/decompiler:
 * `(1.0 - zero_scale_factor) * scale + zero_scale_factor`). */

float effect_scale_factor(float zero_scale_factor, float scale)
{
    return (1.0f - zero_scale_factor) * scale + zero_scale_factor;
}
