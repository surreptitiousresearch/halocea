#ifndef _ID3DXEFFECT_H_
#define _ID3DXEFFECT_H_

/* Direct3DX effect object — COM boundary interface (not decompiled).
   The DB models this as an opaque COM object whose first (and only) word is a
   pointer to the D3DX9 method table (ID3DXEffectVtbl, 79 entries). The prior
   reconstruction inlined a flat, call-order subset of method pointers into this
   struct; that is not the real layout. Restored to the DB/ref { lpVtbl } form.
   The vtable itself is its own ledger entry — forward-declared here. */

/* forward decl — real body is ID3DXEffectVtbl's own header/ledger entry */
typedef struct ID3DXEffectVtbl ID3DXEffectVtbl;

typedef struct ID3DXEffect
{
    ID3DXEffectVtbl *lpVtbl;                     /* 0x0 */
} ID3DXEffect; /* 4 bytes */

/* Full method-table layout (included after the object struct so the vtable's
   ID3DXEffect* parameters resolve). Enables effect->lpVtbl->Method(effect, ...). */
#include "ID3DXEffectVtbl.h"

#endif /* _ID3DXEFFECT_H_ */
