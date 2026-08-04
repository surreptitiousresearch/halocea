#pragma once
/* vidCOMMAND_BUFFER_MANAGER — ws-engine video command-buffer manager.
 * DB-verified layout (types_members vidCOMMAND_BUFFER_MANAGER, 16 bytes) and vtbl
 * (types_members vidCOMMAND_BUFFER_MANAGER_vtbl, 3 slots). */

struct vidCOMMAND_BUFFER; // boundary (pointer only)

typedef struct vidCOMMAND_BUFFER_MANAGER vidCOMMAND_BUFFER_MANAGER;

typedef struct vidCOMMAND_BUFFER_MANAGER_vtbl
{
    void (*dtr_vidCOMMAND_BUFFER_MANAGER)(vidCOMMAND_BUFFER_MANAGER *self, int freeMemory); /* 0x00 deleting dtor (deviation: DB sig has no flag arg; call sites pass it) */
    vidCOMMAND_BUFFER *(*GetCommandBuffer)(vidCOMMAND_BUFFER_MANAGER *self);                /* 0x04 */
    void (*FreeCommandBuffer)(vidCOMMAND_BUFFER_MANAGER *self, vidCOMMAND_BUFFER *);        /* 0x08 */
} vidCOMMAND_BUFFER_MANAGER_vtbl;

struct vidCOMMAND_BUFFER_MANAGER
{
    vidCOMMAND_BUFFER_MANAGER_vtbl *__vftable;     /* 0x00 */
    int                             chunksSkipped; /* 0x04 */
    int                             cmdBufsUsed;   /* 0x08 */
    int                             memUsed;       /* 0x0C */
}; /* 16 bytes */
