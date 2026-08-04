#pragma once
/* vidCMD_BUF_ENV — ws-engine video command-buffer environment (DB types_members vidCMD_BUF_ENV,
 * 20 bytes). */

typedef struct txmTEXTURE txmTEXTURE;

typedef struct vidCMD_BUF_ENV
{
    int         isWriteZ;     /* 0x00 */
    int         isTestZ;      /* 0x04 */
    txmTEXTURE *depthTexture; /* 0x08 */
    txmTEXTURE *lbDir;        /* 0x0C */
    txmTEXTURE *lbCol;        /* 0x10 */
} vidCMD_BUF_ENV; /* 20 bytes */
