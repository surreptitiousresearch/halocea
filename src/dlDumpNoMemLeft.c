extern int noMemLeftInDl;
extern int IGNORE_STRONG_ASSERT;
extern const char empty_string[]; /* shared "" @0x8200155A */

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
