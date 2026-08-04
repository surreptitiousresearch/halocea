/* HCEX_LOADING_SCREEN_DESC::operator= @0x823F4550 — member-wise copy: pBrand, isUnshared,
 * sslClass (via its own ref-counted operator=), isSslClassInited. */

#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h"

HCEX_LOADING_SCREEN_DESC &HCEX_LOADING_SCREEN_DESC::operator=(const HCEX_LOADING_SCREEN_DESC &that)
{
    pBrand = that.pBrand;
    isUnshared = that.isUnshared;
    sslClass = that.sslClass;
    isSslClassInited.val = that.isSslClassInited.val;
    return *this;
}
