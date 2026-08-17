/* ?strCmp@hkString@@SAHPBD0@Z @0x838D3580 */
/* hkString::strCmp — byte-wise C string comparison (unsigned char difference
   of the first mismatch, or of the terminators). */
int hkString_strCmp(const char *a, const char *b)
{
    int diff;
    do
    {
        diff = (int)(unsigned char)*a - (int)(unsigned char)*b;
        if (!*a)
            break;
        ++a;
        ++b;
    }
    while (!diff);
    return diff;
}
