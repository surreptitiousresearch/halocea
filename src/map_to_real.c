/* map_to_real @0x8379C388 — DEVIATION: the decompiler invented an 8-parameter signature packing `range`
 * into a phantom __int64 and adding 4 unused int params plus a phantom trailing __int64, none of which are
 * ever touched. Disasm shows only 4 registers read: r3, r4 (each zero-extended, converted int->float via
 * the standard GPR->stack->FPR fcfid trick), f1, f2. The real computation is
 * `(float)value / (float)range * (upper_bound - lower_bound) + lower_bound` — maps an integer fraction
 * `value/range` onto a float range [lower_bound, upper_bound]. */

float map_to_real(unsigned int value, unsigned int range, float lower_bound, float upper_bound)
{
    return (float)value / (float)range * (upper_bound - lower_bound) + lower_bound;
}
