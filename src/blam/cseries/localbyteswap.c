/* localbyteswap @0x838DDAA8 — swaps the two bytes pointed to by a and b (confirmed via disasm: this is a
 * single-byte pointer swap, not a multi-byte endian byteswap despite the name). */

void localbyteswap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}
