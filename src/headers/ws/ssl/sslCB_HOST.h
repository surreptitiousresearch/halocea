#pragma once
// ssl subsystem: callback host interface. DB-verified layout (types_members sslCB_HOST):
//   __vftable@0 (sslCB_HOST_vtbl *) — size 4. A pure vtable-only host object; the vtable slot
//   layout is external to this batch.

struct sslCB_HOST_vtbl; // boundary — sslCB_HOST virtual table

typedef struct sslCB_HOST {
    sslCB_HOST_vtbl *__vftable; // 0x00
} sslCB_HOST;
