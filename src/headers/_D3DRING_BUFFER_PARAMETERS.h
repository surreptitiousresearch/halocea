#pragma once

/* Xbox 360 D3D command ring-buffer parameters (external library boundary type).
 * DB-verified (types_members _D3DRING_BUFFER_PARAMETERS). */
typedef struct _D3DRING_BUFFER_PARAMETERS
{
    unsigned int Flags;         /* 0x00 */
    unsigned int PrimarySize;   /* 0x04 */
    void *pPrimary;             /* 0x08 */
    unsigned int SecondarySize; /* 0x0C */
    void *pSecondary;           /* 0x10 */
    unsigned int SegmentCount;  /* 0x14 */
} _D3DRING_BUFFER_PARAMETERS; /* 24 bytes */
