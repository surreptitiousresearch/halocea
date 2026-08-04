/* hcex_decals_clear @0x823D68B8 — clear all bridged decals and reset the ws-engine scorch and grass systems
 * when a map (re)loads. The ws-engine container/manager classes are boundary types (the outer host engine);
 * their methods are declared, not reversed. */

/* ws-engine boundary types and singletons. */
typedef struct HCEX_DECAL HCEX_DECAL;
typedef struct dsVECTOR_HCEX_DECAL_8 dsVECTOR_HCEX_DECAL_8;
typedef struct scrSCORCH_MANAGER scrSCORCH_MANAGER;
typedef struct grsGRASS grsGRASS;

extern dsVECTOR_HCEX_DECAL_8 hcexCreateDecals;
extern scrSCORCH_MANAGER *scrManager;
extern grsGRASS *_grsEntity;

/* dsVECTOR<HCEX_DECAL,8>::Clear, scrSCORCH_MANAGER::RemoveAll/UnfarmAll, grsGRASS::UnhideAllGrass — ws-engine
 * methods, boundary (not reversed). */
extern void dsVECTOR_HCEX_DECAL_8__Clear(dsVECTOR_HCEX_DECAL_8 *self);
extern void scrSCORCH_MANAGER_RemoveAll(scrSCORCH_MANAGER *self);
extern void scrSCORCH_MANAGER_UnfarmAll(scrSCORCH_MANAGER *self);
extern void grsGRASS_UnhideAllGrass(grsGRASS *self);

extern "C" void hcex_decals_clear(void)
{
    dsVECTOR_HCEX_DECAL_8__Clear(&hcexCreateDecals);
    if ( scrManager )
    {
        scrSCORCH_MANAGER_RemoveAll(scrManager);
        scrSCORCH_MANAGER_UnfarmAll(scrManager);
    }
    if ( _grsEntity )
        grsGRASS_UnhideAllGrass(_grsEntity);
}
