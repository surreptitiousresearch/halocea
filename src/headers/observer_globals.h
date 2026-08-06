/* observer_globals — the camera/observer state for the local players. The per-local-player `observer`
 * record is defined in observer.h; this header only adds the global container. */
#pragma once

#include "observer.h"

/* DB anonymous struct: types_members _F2B31F9836BA4BB4C491B25A844109E4
 * (note: _15F3ED9EB994D8D3C43F28ECD84A8394 is the director globals, not this) */
typedef struct observer_globals_t
{
    float    dtime;               /* 0x00 */
    observer local_players[2];    /* 0x04 */
} observer_globals_t;

#ifdef __cplusplus
extern "C" {
#endif

extern observer_globals_t observer_globals;

#ifdef __cplusplus
}
#endif
