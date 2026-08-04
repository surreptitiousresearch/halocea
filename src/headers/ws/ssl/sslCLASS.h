#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "sslCLASS_REF.h"
#include "sslSOURCE_REF.h"
#include "sslCLASS_ELEMENT.h"
// ssl subsystem: a script class descriptor. DB-verified layout (types_members sslCLASS):
//   dsREF_COUNT@0, pSslSystem@4, refParent@8, stateClass@12, nameToElemIdx@16, elements@36,
//   events@56, numStoredMembers@76, pBitEventsTbl@80, dbgInfo@84, source@88, uid@92 — size 96.
// The dsVECTOR members store T* (pointer), so their element types stay incomplete here.

struct sslSYSTEM;                 // boundary — owning script system
struct sslOBJECT;                 // ssl script object instance (full definition in sslOBJECT.h)
struct sslERROR;                  // ssl error/result value (full definition in sslERROR.h)
struct dsDATA;                    // ds type-erased value (full definition in ds/dsDATA.h)
struct sslCLASS_BIT_EVENTS_TBL;   // boundary — per-class bit-event table
struct sslFUNCTION_DECL;          // function/event signature (full definition in sslFUNCTION_DECL.h)
namespace ssl_class {
    struct NAME_TO_IDX;           // boundary — sslCLASS::NAME_TO_IDX vector element
}
// sslCLASS_ELEMENT — full definition in sslCLASS_ELEMENT.h (the elements-vector element type).

typedef struct sslCLASS {
    dsREF_COUNT                             refCount;         // 0x00
    sslSYSTEM                              *pSslSystem;       // 0x04 owning script system
    sslCLASS_REF                           refParent;        // 0x08 parent class
    apSTATE_T<unsigned long>               stateClass;       // 0x0C class load/parse state
    dsVECTOR<ssl_class::NAME_TO_IDX, 8>    nameToElemIdx;    // 0x10 element-name -> index map
    dsVECTOR<sslCLASS_ELEMENT, 8>          elements;         // 0x24 member/field descriptors
    dsVECTOR<dsSTRID, 8>                   events;           // 0x38 declared event ids
    int                                    numStoredMembers; // 0x4C
    sslCLASS_BIT_EVENTS_TBL               *pBitEventsTbl;    // 0x50
    dsTSTRING<char>                        dbgInfo;          // 0x54
    sslSOURCE_REF                          source;           // 0x58 ref to this class's source text
    int                                    uid;              // 0x5C unique class id

    // 0x82A378C0 — true if this class is `pClass` or transitively derives from it. const (QBA).
    int IsSubClassOf(sslCLASS *pClass) const;
    // 0x82523120 — copy of this class's source-text reference (returns sslSOURCE_REF by value). QAA.
    sslSOURCE_REF GetSource();
    // 0x82A3B9C0 (?LockElems@sslCLASS@@QAAXXZ) — finalize/lock the class's element table after all
    // members and scripts have been added.  boundary.
    void LockElems();

    // Sub-element (member) predicates: `idx` indexes the elements table. The binary uses an
    // inclusive `idx <= elements.nElem` upper bound (reproduced faithfully). const (QBA).
    // 0x82A388E8 — element is a function (native or scripted).
    int IsSubElemFunc(int idx) const;
    // 0x82A389B0 — element is an event handler.
    int IsSubElemEvent(int idx) const;
    // 0x82A38A78 — element is a variable (data member). const (QBA).
    int IsSubElemVar(int idx) const;
    // 0x82A38AE0 — element is a constant / base variable.
    int IsSubElemConst(int idx) const;
    // 0x82A38B40 — name of sub-element `idx`, or a null dsSTRID if out of range (returns by value). QBA.
    dsSTRID GetSubElemName(int idx) const;

    // 0x82A38870 — element index of the sub-element named `name`, searching this class and its
    // parents; -1 if not found. QAA.
    int GetSubNameIdx(dsSTRID name);
    // 0x82A3AD78 — the sslFUNCTION_DECL of function/event element `idx` (null if `idx` is not a
    // function or event). QAA.
    sslFUNCTION_DECL *GetFunctionDecl(int idx);

    // 0x82A398D0 — invoke native-callback element `idx` on `pSelf` with (argc,argv)->retVal, made on
    // behalf of `pCaller`; returns an sslERROR by value (sret). Skips (returns no-error) when the
    // object is still constructing and the callback is not flagged run-during-construct. Protected (IAA).
protected:
    sslERROR CallCbFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv, dsDATA &retVal, sslOBJECT *pCaller);

    // 0x82A39A18 — invoke scripted-function element `idx` on `pSelf` with (argc,argv)->retVal on
    // behalf of `pCaller`: push the arguments onto the owning system's stack, record the function
    // name for debugging, then evaluate the compiled function. Skips (returns no-error) when the
    // object is still constructing and the function is not flagged run-during-construct. Returns
    // sslERROR by value (sret). Protected (IAA).
    sslERROR CallScriptedFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv, dsDATA &retVal, sslOBJECT *pCaller);
public:

    // 0x82A3BA50 — public function dispatch: invoke function/callback/event element `idx` on `pSelf`
    // with (argc,argv)->retVal, on behalf of `pCaller`; returns an sslERROR by value (sret).
    sslERROR CallFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv, dsDATA *retVal, sslOBJECT *pCaller);

    // (?HandleEvent@sslCLASS@@QAA?AVsslERROR@@PAVsslOBJECT@@HHPAPBVdsDATA@@@Z) — dispatch event `idx`
    // on `pSelf` with (argc, argv); returns an sslERROR by value (sret). Boundary from the ssl-ref
    // re-source (called by sslOBJ_REF::HandleEvent, one level past it).
    sslERROR HandleEvent(sslOBJECT *pSelf, int idx, int argc, dsDATA *const *argv);

    // 0x82A38CA0 — store `val` into settable member element `idx` of `pSelf` (converting to the
    // member's declared type); returns sslERROR by value (sret). QAA.
    sslERROR SetMemberValue(sslOBJECT *pSelf, int idx, const dsDATA &val);
    // 0x82A39140 — read member element `idx` of `pSelf` into `val` (converting to its declared type);
    // returns sslERROR by value (sret). QAA.
    sslERROR GetMemberValue(sslOBJECT *pSelf, int idx, dsDATA &val);

    // 0x82A3AA30 — tear down every OWNED (non-inherited) element per its TYPE (native callback /
    // lone scripted function / chained event-handler list / polymorphic base-var, virtual delete),
    // free the bit-events table if allocated, notify the owning system (_OnClassDelete), and
    // release dbgInfo's string buffer. events, elements, nameToElemIdx, source, and refParent are
    // all real members with their own already-reversed, non-trivial destructors, so the
    // compiler-generated member epilogue tears them down automatically (see the CAVEAT in
    // sslCLASS__dtor.cpp) -- not written explicitly in the reconstructed body.
    ~sslCLASS();
} sslCLASS;

// Process-wide count of live sslCLASS instances, decremented in the destructor (DB symbol __sslNClass).
extern int __sslNClass;
