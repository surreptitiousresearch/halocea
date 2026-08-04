#pragma once
/* Boundary declarations for hcex_obj_term_instances.c.
 *
 * hcex_obj_term_instances tears down every tracked HCEX_OBJ's animated ws-engine instance at once
 * (used when the object-sync system itself is being torn down, e.g. on a map change): for every
 * entry whose instance has already lost its entity (pEnt == null, i.e. the entity side was already
 * destroyed elsewhere), it destroys the scene instance directly; every entry's pInst is then cleared
 * regardless. */

#include "HCEX_OBJ.h"
#include "hcex_obj_customize_boundary.h"   /* animINST (pEnt @0x0C, flat/opaque) */

#include "../ws/ds/dsVECTOR.h"
typedef dsVECTOR<HCEX_OBJ, 8> dsVECTOR_HCEX_OBJ_8; /* DB dsVECTOR<HCEX_OBJ,8> */
extern dsVECTOR_HCEX_OBJ_8 hcexObjects;

typedef struct scnSCENE scnSCENE;
extern scnSCENE *gsScenePtr;
extern void scnSCENE_DestroyInst(scnSCENE *scene, animINST *instance);   /* already used by hcex_destroy_dead_intances.c */

extern int IGNORE_STRONG_ASSERT;
