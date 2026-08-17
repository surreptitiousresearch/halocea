/* processFlyingColorsDestruction @0x83A4D890 */
/* processFlyingColorsDestruction — Havok Destruction "flying colors" evaluation-
   key check. Identical structure to processFlyingColors but over the Destruction
   key globals (HK_DESTRUCTION_KEYCODE / HK_DESTRUCTION_KEYVALUE) and the
   Destruction product magic. The embedded key string is scanned past its first
   two '.'-delimited fields; the trailing field is folded with h = 23*h + c and
   compared against the licence value, or (sign bit set) treated as a time-limited
   expiry stamp versus the high word of hkGetSystemTime(). Returns 1 when valid,
   otherwise emits the expiry message and returns 0. */

extern const char HK_DESTRUCTION_KEYCODE[];
extern int HK_DESTRUCTION_KEYVALUE;
extern unsigned long long hkGetSystemTime(void);
extern void hkErrorMessage(const char *message);

int processFlyingColorsDestruction(void)
{
    const char *p = "0x49f074ff:CLIENT.De.SaberZeroG";

    if (HK_DESTRUCTION_KEYCODE[0] != '.')
    {
        int c = HK_DESTRUCTION_KEYCODE[0];
        while (c)
        {
            c = *++p;
            if (c == '.')
                goto found_first;
        }
        goto invalid;
    }
found_first:

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

    if ((HK_DESTRUCTION_KEYVALUE & 0x80000000) == 0)
    {
        int h = 0;
        for (; *p; ++p)
            h = 23 * h + *p;
        if (HK_DESTRUCTION_KEYVALUE == ((h & 0x7FFFFFFF) ^ 0x8C02578))
            return 1;
    }
    else
    {
        int expiry = (HK_DESTRUCTION_KEYVALUE & 0x7FFFFFFF) ^ 0x8C02578;
        int now = (int)(hkGetSystemTime() >> 32);
        if (expiry > now && expiry - now < 34186669)
            return 1;
    }

invalid:
    hkErrorMessage("Havok Destruction evaluation key has expired or is invalid.\n"
                   "Please contact Havok.com for an extension.\n"
                   "No simulation possible.");
    return 0;
}
