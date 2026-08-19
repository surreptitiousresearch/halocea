/* hs_real_to_long @0x8368D600 */
/* DEVIATION: type-punned read — r is an integer register holding a float bit-pattern.
 * The decompiler renders this as (int)*(float*)&r; we preserve that idiom. */
int hs_real_to_long(int r)
{
    return (int)*(float *)&r;
}
