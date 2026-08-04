/* processFlyingColors — Havok Physics "flying colors" evaluation-key check. The
   embedded key string is scanned past its first two '.'-delimited fields; the
   trailing field is folded with the hash h = 23*h + c and compared against the
   licence value HK_PHYSICS_KEYVALUE. When the value's sign bit is set it is a
   time-limited key instead: the low 31 bits (xor the product magic) are treated
   as an expiry stamp compared against the high word of hkGetSystemTime(). Returns
   1 when the key is valid/unexpired, otherwise emits the standard Havok expiry
   message and returns 0. sourceComponent is unused (matches the DB body). */

extern const char HK_PHYSICS_KEYCODE[];
extern int HK_PHYSICS_KEYVALUE;
extern unsigned long long hkGetSystemTime(void);
extern void hkErrorMessage(const char *message);

int processFlyingColors(int sourceComponent)
{
    const char *p = "0x33d6bed6:CLIENT.Ph.SaberZeroG";
    (void)sourceComponent;

    /* Advance past the first '.' field. */
    if (HK_PHYSICS_KEYCODE[0] != '.')
    {
        int c = HK_PHYSICS_KEYCODE[0];
        while (c)
        {
            c = *++p;
            if (c == '.')
                goto found_first;
        }
        goto invalid;
    }
found_first:

    /* Advance past the second '.' field. */
    ++p;
    if (*p != '.')
    {
        int c = *p;
        while (c)
        {
            c = *++p;
            if (c == '.')
                goto found_second;
        }
        goto invalid;
    }
found_second:
    ++p;

    if ((HK_PHYSICS_KEYVALUE & 0x80000000) == 0)
    {
        int h = 0;
        for (; *p; ++p)
            h = 23 * h + *p;
        if (HK_PHYSICS_KEYVALUE == ((h & 0x7FFFFFFF) ^ 0x72E6EF51))
            return 1;
    }
    else
    {
        int expiry = (HK_PHYSICS_KEYVALUE & 0x7FFFFFFF) ^ 0x72E6EF51;
        int now = (int)(hkGetSystemTime() >> 32);
        if (expiry > now && expiry - now < 34186669)
            return 1;
    }

invalid:
    hkErrorMessage("Havok Physics evaluation key has expired or is invalid.\n"
                   "Please contact Havok.com for an extension.\n"
                   "No simulation possible.");
    return 0;
}
