#ifndef _MEMORY_BASIC_INFORMATION_H
#define _MEMORY_BASIC_INFORMATION_H

/* Win32 MEMORY_BASIC_INFORMATION boundary type (declared locally to avoid a
 * <windows.h> dependency that collides with the reconstructed Blam Win32 stub
 * headers). Standard Win32 layout, 32-bit variant (28 bytes as queried). */
typedef struct _MEMORY_BASIC_INFORMATION
{
    void          *BaseAddress;       /* 0x00 */
    void          *AllocationBase;    /* 0x04 */
    unsigned long  AllocationProtect; /* 0x08 */
    unsigned long  RegionSize;        /* 0x0C */
    unsigned long  State;             /* 0x10 */
    unsigned long  Protect;           /* 0x14 */
    unsigned long  Type;              /* 0x18 */
} MEMORY_BASIC_INFORMATION;

#endif /* _MEMORY_BASIC_INFORMATION_H */
