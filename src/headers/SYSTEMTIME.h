#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H

/* Win32 SYSTEMTIME boundary type (declared locally to avoid a <windows.h>
 * dependency that collides with the reconstructed Blam Win32 stub headers).
 * Standard Win32 layout: 8 WORDs, 16 bytes. */
typedef struct _SYSTEMTIME
{
    unsigned short wYear;         /* 0x00 */
    unsigned short wMonth;        /* 0x02 */
    unsigned short wDayOfWeek;    /* 0x04 */
    unsigned short wDay;          /* 0x06 */
    unsigned short wHour;         /* 0x08 */
    unsigned short wMinute;       /* 0x0A */
    unsigned short wSecond;       /* 0x0C */
    unsigned short wMilliseconds; /* 0x0E */
} SYSTEMTIME;

#endif /* SYSTEMTIME_H */
