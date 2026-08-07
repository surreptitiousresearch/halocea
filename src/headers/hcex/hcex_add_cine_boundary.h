#pragma once
/* Boundary declarations for cbAddCine (SSL callback "AddCine"). The cinematics-in-progress registry
 * `listCine` maps a cinematic name to a per-difficulty table of (actor, mask) records; AddCine boxes
 * the calling object's cinematic actor and its difficulty mask into that registry. The ws ssl / ds
 * containers are canonical headers; only the HCEX global and the ds::Tokenize helper are declared
 * here. Source: D:\Projects\code\HCEX\sources\halo_cine.cpp. */

#include "../ws/ssl/sslOBJ_REF.h"                 /* sslOBJ_REF */
#include "../ws/ssl/sslCB_HOST.h"                 /* sslCB_HOST */
#include "ssl_exporter/sslOBJ_REF_ssl_export_bridge.h" /* sslOBJ_REF_bridge::GetCbHost */
#include "../ws/ds/dsDATA.h"                      /* dsDATA */
#include "../ws/ds/dsDATA_TYPE.h"                 /* dsDATA_TYPE::GetPtr */
#include "../ws/ds/dsTSTRING.h"                   /* dsTSTRING<char> */
#include "../ws/ds/dsVECTOR.h"                    /* dsVECTOR<T,8> (InsertSorted/PushBack/operator[]) */
#include "../ws/ds/dsPAIR.h"                      /* dsPAIR<K,V> */
#include "../ws/gs/gsANITEC_ACTOR.h"              /* gsANITEC_ACTOR (the cinematic actor / cb host) */
#include "hcexCINE_INFO.h"                        /* hcexCINE_INFO (actor + apSTATE_T<unsigned char> mask) */

/* stateless comparator selector for InsertSorted (an empty predicate; DB dsCMP). */
#include "../ws/ds/dsCMP.h"   /* canonical dsCMP (stateless empty struct) — a local `typedef int dsCMP` is a C2371/ODR conflict with it */

/* listCine element: cinematic name -> per-difficulty (actor,mask) table.
 * DB type: dsVECTOR<dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>,8>. */
typedef dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > hcex_cine_entry;
extern dsVECTOR<hcex_cine_entry, 8> listCine;

/* dlmalloc free — used to release the transient InsertSorted argument buffers. */
extern "C" void dlFree(void *ptr);

namespace ds {
/* ds::DEF_TOKEN_PROCESSOR — default token collector: appends each token into a caller-supplied
 * dsVECTOR<dsTSTRING<char>,8>. boundary. */
struct DEF_TOKEN_PROCESSOR;
/* ds::Tokenize — split `text` on occurrences of `delim`, feeding each token to `processor`. boundary. */
template<class PROCESSOR>
void Tokenize(dsTSTRING<char> *text, dsTSTRING<char> *delim, PROCESSOR *processor);
}
