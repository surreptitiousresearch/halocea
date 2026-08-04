/* DEVIATION: type-punned read — r is an integer register holding a float bit-pattern.
 * The decompiler renders this as (int)*(float*)&r; we preserve that idiom. */
int hs_real_to_long(int r)
{
    return (int)*(float *)&r;
}
