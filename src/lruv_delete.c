/* lruv_delete @0x8371C0F8 */
#include "headers/lruv_cache.h"
#include <string.h>

extern void dlFree(void *ptr);

void lruv_delete(lruv_cache *cache)
{
    memset(cache, 0, sizeof(lruv_cache));
    dlFree(cache);
}
