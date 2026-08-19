/* ufclose @0x8376D8C0 — thunk: close a stdio stream (tail-jump `b fclose`).
 * Attestation: 0/6 callers consume r3 — declared return is void. */

#include <stdio.h>

void ufclose(FILE *stream)
{
    fclose(stream);
}
