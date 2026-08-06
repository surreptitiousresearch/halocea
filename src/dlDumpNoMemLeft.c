extern int noMemLeftInDl;
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

void apDumpNoMemLeft(void);
void STRONG_ASSERT_DUMMY_Crash(void *unused, const char *msg, const char *file, int line, const char *info);

void dlDumpNoMemLeft(void)
{
    noMemLeftInDl = 1;
    apDumpNoMemLeft();
    if (!IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY_Crash(
            0,
            "!\"No mem left in dl alloc\"",
            "D:\\Projects\\code\\common\\src.sys\\Ap\\ap_dlalloc.cpp",
            150,
            empty_string);
}
