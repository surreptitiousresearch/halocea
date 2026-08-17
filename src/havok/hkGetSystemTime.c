/* hkGetSystemTime @0x83FD8C98 */
/* hkGetSystemTime — wall-clock seconds since the epoch as a 64-bit value; a thin
   wrapper over the CRT _time64(). _time64/__time64_t are the CRT boundary. */
extern long long _time64(long long *timer); /* _time64 (CRT boundary) */

long long hkGetSystemTime(void)
{
    return _time64((long long *)0);
}
