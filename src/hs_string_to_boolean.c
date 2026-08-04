/* hs_string_to_boolean @0x8368D528 — HaloScript type coercion: a string value coerces to the boolean TRUE
 * only when it is the empty string (length 0), otherwise FALSE.
 * DEVIATION: the binary computes strlen then packs (cntlzw(length) & 0x20) != 0 — true iff length == 0 —
 * into the MSByte of the back-chain slot and reads the whole word back (big-endian).
 * DEVIATION: the parameter is the raw `int` script value word, which this coercion uses as the text
 * pointer (`mr r11, r3` / `lbz r10, 0(r11)`). The typecasting_procedures ABI is word-in/word-out. */

int hs_string_to_boolean(int value)
{
    const char *string = (const char *)value;
    const char *cursor = string;
    while (*cursor++)
        ;
    long length = (long)(cursor - string) - 1;

    int back_chain;
    *(unsigned char *)&back_chain = (length == 0) ? 1 : 0;
    return back_chain;
}
