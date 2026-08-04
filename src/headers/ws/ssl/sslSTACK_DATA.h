#pragma once
// ssl subsystem: one entry on the script evaluation stack. DB-verified layout
// (types_members sslSTACK_DATA): type@0 (TYPE), anonymous union@4 (12 bytes) — size 16.
// The TYPE discriminator and every union arm are DB-verified (ws_ssl_0002): the union
// aliases three scalar arms (int/bool/float) plus seven reference records and a raw
// 12-byte `data` arm. Pointer/index arms are 8-12 bytes each.

struct sslSTACK;   // ssl evaluation/local stack (full definition in sslSTACK.h)
struct sslOBJECT;  // ssl script object instance (full definition in sslOBJECT.h)
struct sslCLASS;   // ssl script class descriptor (full definition in sslCLASS.h)
struct sslSYSTEM;  // ssl script system (full definition in sslSYSTEM.h)
struct dsDATA;     // ds type-erased value (full definition in ds/dsDATA.h)
struct dsSTRID;    // ds interned string id (full definition in ds/dsSTRID.h)
struct sslERROR;   // ssl error/result value (full definition in sslERROR.h)

/* Union arm records. The DB spells these as anonymous inline aggregates
 * (<unnamed_type_dsDataMemberRef> etc.), so they have no DB type name of their
 * own; SSL_UNNAMED_RECORD keeps the layout verifier (which keys aggregates by
 * their spelled keyword+name) from mis-binding these helper spellings to the
 * file's basename type. */
#define SSL_UNNAMED_RECORD struct
SSL_UNNAMED_RECORD DS_DATA_MEMBER_REF { void *ds_data[2]; const char *id; };                        // 12 — TYPE_DSDATA_MEMBER_REF
SSL_UNNAMED_RECORD OBJ_REF            { struct sslOBJECT *pObj; struct sslCLASS *pClass; };         // 8 — TYPE_SSLOBJECT_REF
SSL_UNNAMED_RECORD SUB_ELEM_REF       { struct sslOBJECT *pObj; struct sslCLASS *pObjClass; int idx; }; // 12 — TYPE_SSLOBJECT_SUBELEM_REF
SSL_UNNAMED_RECORD FUNC_PARAM_STACK   { struct sslSTACK *pStack; int idx; };  // 8 — TYPE_SSLFUNC_PARAM_REF_STACK
SSL_UNNAMED_RECORD LOCAL_VAR          { struct sslSTACK *pStack; int idx; };  // 8 — TYPE_SSLFUNC_LOCAL_VAR_REF
SSL_UNNAMED_RECORD FUNC_PARAM_ARR     { struct dsDATA *pTbl; int idx; };      // 8 — TYPE_SSLFUNC_PARAM_REF_ARR
SSL_UNNAMED_RECORD FUNC_CONST         { struct dsDATA *pTbl; int idx; };      // 8 — TYPE_SSLFUNC_CONST_REF

typedef struct sslSTACK_DATA {
    enum TYPE {
        TYPE_INT                     = 0,
        TYPE_BOOL                    = 1,
        TYPE_FLOAT                   = 2,
        TYPE_DSSTRID                 = 3,
        TYPE_DS_DATA                 = 4,
        TYPE_SSLOBJECT_REF           = 5,
        TYPE_SSLOBJECT_SUBELEM_REF   = 6,
        TYPE_SSLFUNC_PARAM_REF_STACK = 7,
        TYPE_SSLFUNC_PARAM_REF_ARR   = 8,
        TYPE_SSLFUNC_CONST_REF       = 9,
        TYPE_SSLFUNC_LOCAL_VAR_REF   = 10,
        TYPE_DSDATA_MEMBER_REF       = 11,
    };

    // 0x82AF0D70 — placement-copy-construct helper: `template<> sslSTACK_DATA *New<sslSTACK_DATA>
    // (sslSTACK_DATA *place, const sslSTACK_DATA *val)` in sslSTACK_DATA__New.cpp (specializes the
    // generic New<T> placement-copy helper used across the codebase; not a member function).

    TYPE type; // 0x00 payload discriminator

    // 0x04 payload (DB anonymous union _11CA420B58F32FE7952B21B45CCF134B, 12 bytes) — arms
    // injected into struct scope so .cpp bodies access them directly (int_val, objRef, ...).
    union {
        int                int_val;         // 0x00 TYPE_INT / TYPE_DSSTRID scalar slot
        bool               bool_val;        // 0x00 TYPE_BOOL
        float              float_val;       // 0x00 TYPE_FLOAT
        struct DS_DATA_MEMBER_REF dsDataMemberRef; // 0x00
        struct OBJ_REF            objRef;          // 0x00
        struct SUB_ELEM_REF       subElemRef;      // 0x00
        struct FUNC_PARAM_STACK   funcParamStack;  // 0x00
        struct LOCAL_VAR          localVar;        // 0x00
        struct FUNC_PARAM_ARR     funcParamArr;    // 0x00
        struct FUNC_CONST         funcConst;       // 0x00
        unsigned int       data[3];         // 0x00 raw 12-byte payload (covers TYPE_DS_DATA in-place dsDATA)
    };

    // 0x82AF05F8 — drop the current payload, leaving the slot empty: for object/sub-element refs
    // atomically release the object and class ref-counts; for an in-place dsDATA (TYPE_DS_DATA /
    // TYPE_DSDATA_MEMBER_REF) dispatch the value's runtime-type Destroy. Scalar/index arms need no
    // teardown. Called at the head of every SetTo* builder. Protected (IAA).
protected:
    void Cleanup();
public:

    // 0x82AF06B0 — set to an int value.
    sslSTACK_DATA &SetToInt(int val);
    // 0x82AF06F8 — set to a bool value.
    sslSTACK_DATA &SetToBool(bool val);
    // 0x82AF0740 — set to a float value.
    sslSTACK_DATA &SetToFloat(float val);
    // 0x82AF0788 — set to an interned string id (payload stored in the scalar slot).
    sslSTACK_DATA &SetToDsStrId(dsSTRID id);
    // 0x82AF0B88 — set to a copy of a type-erased dsDATA value (constructed in place).
    sslSTACK_DATA &SetToDsData(const dsDATA &val);
    // 0x82AF07D0 — set to an object reference (adds a ref to both object and class).
    sslSTACK_DATA &SetToObjRef(sslOBJECT *obj, sslCLASS *cls);
    // 0x82AF0888 — set to a by-stack function-parameter reference.
    sslSTACK_DATA &SetToFuncParamRefStack(sslSTACK *pStack, int idx);
    // 0x82AF08C0 — set to a local-variable reference.
    sslSTACK_DATA &SetToLocalVarRef(sslSTACK *pStack, int idx);
    // 0x82AF0930 — set to a function-constant-table reference.
    sslSTACK_DATA &SetToFuncConstRef(dsDATA *pTbl, int idx);
    // 0x82AF0828 — set to an object sub-element (member) reference (adds a ref to object and class).
    sslSTACK_DATA &SetToObjSubElemRef(sslOBJECT *obj, sslCLASS *cls, int idx);
    // 0x82AF0BD8 — set to a dsDATA member reference: copy the dsDATA in place and record the member id.
    sslSTACK_DATA &SetToDsDataMemberRef(const dsDATA &val, dsSTRID id);

    // 0x82AF0EF0 — read out a TYPE_DSDATA_MEMBER_REF payload: copy the referenced dsDATA into `val`
    // and its member id into `id`; returns a (no-error) sslERROR by value (sret). const (QBA).
    sslERROR GetAsDsDataMemberRef(dsDATA &val, dsSTRID &id) const;

    // 0x82AF2D20 — read out this value as a dsDATA into `out`; returns a no-error sslERROR on
    // success or a describing error otherwise. const (QBA).
    sslERROR GetAsDsData(dsDATA &out) const;

    // 0x82AF27D0 — assign `val` through this reference-kind slot (sub-element / local-var / struct
    // member); read-only kinds and non-reference slots yield an error. Returns sslERROR by value
    // (sret). QAA.
    sslERROR SetRefTo(const dsDATA &val);

    // 0x82AF0E08 — read out a TYPE_SSLOBJECT_SUBELEM_REF payload into (obj,cls,idx); on a type
    // mismatch returns a formatted sslERROR describing the failed conversion (by value / sret).
    // `pSsl` is accepted for signature parity but unused by the reversed body. const (QBA).
    sslERROR GetAsObjSubElemRef(sslOBJECT *&obj, sslCLASS *&cls, int &idx, sslSYSTEM *pSsl) const;

    // 0x82AF3468 — read this stack value as a bool into `val`: fetch its dsDATA form, then either
    // propagate the fetch error, or convert to bool (SSL_ERR_RUNTIME on a failed conversion).
    // Returns sslERROR by value (sret). const (QBA).
    sslERROR GetAsBool(bool &val) const;
    // 0x82AF3760 — read this stack value as a float into `val` (see GetAsBool). const (QBA).
    sslERROR GetAsFloat(float &val) const;
    // 0x82AF3A58 — read this stack value as an int into `val` (see GetAsBool). const (QBA).
    sslERROR GetAsInt(int &val) const;
    // 0x82AF41B8 — read this stack value as an object reference into (obj,cls): direct for an
    // object-ref payload, else fetch its dsDATA form and convert via sslOBJ_REF::Set. Returns
    // sslERROR by value (sret): SSL_ERR_GENERIC when the payload cannot be an object ref. const (QBA).
    sslERROR GetAsObjRef(sslOBJECT *&obj, sslCLASS *&cls, sslSYSTEM *pSsl) const;

    // 0x82AF3D50 — convert this stack value in place to a TYPE_DS_DATA holding its dsDATA form; a
    // no-op when already TYPE_DS_DATA. Returns a no-error sslERROR on success, else the conversion
    // error (by value / sret). QAA.
    sslERROR ConvertToDsData();

    // Default: an empty TYPE_INT/0 slot (matches the copy-ctor's default-init prologue). Inline so
    // callers that create a fresh entry (sslSTACK::Push) and the shared empty defaults compile.
    sslSTACK_DATA() { type = TYPE_INT; int_val = 0; }
    // 0x82AF0CD0 — copy-construct: default-init to TYPE_INT/0 then delegate to operator=.
    sslSTACK_DATA(const sslSTACK_DATA &val);

    // sslSTACK::Push releases a temporary entry's payload via Cleanup() (protected) after the vector
    // takes its own copy — grant it access rather than widening Cleanup's visibility.
    friend struct sslSTACK;
    // dsVECTOR<sslSTACK_DATA,8>::Clear calls the protected Cleanup() directly (as the original
    // source does); grant its container access rather than widening Cleanup's visibility.
    template<class T, int N> friend struct dsVECTOR;
    // 0x82AF0AA0 — copy-assign: ref-count-aware for TYPE_DS_DATA (placement-copy the in-place dsDATA),
    // TYPE_SSLOBJECT_REF and TYPE_SSLOBJECT_SUBELEM_REF (bump the object/class refs); every other
    // payload kind is a raw union copy.
    sslSTACK_DATA &operator=(const sslSTACK_DATA &val);
    // 0x82AF0B80 — thunk to Cleanup().
    ~sslSTACK_DATA();
} sslSTACK_DATA;
