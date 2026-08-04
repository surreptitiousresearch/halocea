/* main_crash @0x8368AE58 — deliberately crash by writing through a null pointer (debug "force crash").
 *
 * Deviation: decompiler renders the null-deref as MEMORY[0] = ...; reproduced as a write through address 0. */

void main_crash(const char *str)
{
    (void)str;
    *(const char **)0 = "chucky was here!  NULL belongs to me!!!!!";
}
