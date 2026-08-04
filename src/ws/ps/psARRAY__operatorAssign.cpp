#include "psARRAY.h"
#include "psARRAY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
// 0x827AEECC — referenced copy-assign: bump `src`'s backing-store refcount, release this handle's
// prior backing store (freeing it at zero), then adopt `src`'s pointer.

psARRAY &psARRAY::operator=(const psARRAY &src)
{
    if (src.pData)
        ++src.pData->refCount;

    psARRAY_DATA *data = this->pData;
    if (data) {
        if (--data->refCount == 0) {
            data->data.~dsVECTOR<dsDATA, 8>();
            operator delete(data);
        }
    }

    this->pData = src.pData;
    return *this;
}
