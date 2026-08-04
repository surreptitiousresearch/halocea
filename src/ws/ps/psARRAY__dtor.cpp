#include "psARRAY.h"
#include "psARRAY_DATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
// 0x827AEE80 — destructor: drop a reference on the backing store, freeing its element vector and
// itself at zero.

psARRAY::~psARRAY()
{
    psARRAY_DATA *data = this->pData;
    if (data) {
        if (--data->refCount == 0) {
            data->data.~dsVECTOR<dsDATA, 8>();
            operator delete(data);
        }
    }
}
