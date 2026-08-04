#pragma once
#include "hkReferencedObject.h"
#include "hkBool.h"
#include "hkArray.h"
#include "hkPadSpu.h"
#include "hkEnum.h"
#include "hkpShapeType.h"

typedef struct hkpCdBody hkpCdBody;
typedef struct hkpCollisionInput hkpCollisionInput;
typedef struct hkpLinearCastCollisionInput hkpLinearCastCollisionInput;
typedef struct hkpContactMgr hkpContactMgr;
typedef struct hkpContactMgrFactory hkpContactMgrFactory;
typedef struct hkpCdPointCollector hkpCdPointCollector;
typedef struct hkpCdBodyPairCollector hkpCdBodyPairCollector;
typedef struct hkpCollisionAgent hkpCollisionAgent;
typedef struct hkpAgent3Input hkpAgent3Input;
typedef struct hkpAgent3ProcessInput hkpAgent3ProcessInput;
typedef struct hkpAgentEntry hkpAgentEntry;
typedef struct hkpConstraintOwner hkpConstraintOwner;
typedef struct hkpProcessCollisionOutput hkpProcessCollisionOutput;
typedef struct hkStatisticsCollector hkStatisticsCollector;
typedef struct hkVector4 hkVector4;

/* types_enum_values hkAgent3::Symmetric */
enum Symmetric
{
    IS_SYMMETRIC = 0,
    IS_NOT_SYMMETRIC = 1,
    IS_NOT_SYMMETRIC_AND_FLIPPED = 2,
};

/* types_members hkpCollisionDispatcher::AgentFuncs size 20. Only m_linearCastFunc
   is invoked by the reversed caller. */
typedef struct hkpCollisionDispatcher_AgentFuncs
{
    hkpCollisionAgent *(*m_createFunc)(const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpContactMgr *); /* 0  */
    void (*m_getPenetrationsFunc)(const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpCdBodyPairCollector *); /* 4 */
    void (*m_getClosestPointFunc)(const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpCdPointCollector *); /* 8 */
    void (*m_linearCastFunc)(const hkpCdBody *castBody, const hkpCdBody *fixedBody, const hkpLinearCastCollisionInput *input, hkpCdPointCollector *castCollector, hkpCdPointCollector *startCollector); /* 12 */
    hkBool m_isFlipped;      /* 16 */
    hkBool m_isPredictive;   /* 17 */
} hkpCollisionDispatcher_AgentFuncs;

/* types_members hkpCollisionDispatcher::Agent3Funcs size 64 */
typedef struct hkpCollisionDispatcher_Agent3Funcs
{
    void *(*m_createFunc)(const hkpAgent3Input *, hkpAgentEntry *, void *);                                                        /* 0x00 */
    void (*m_destroyFunc)(hkpAgentEntry *, void *, hkpContactMgr *, hkpConstraintOwner *, struct hkpCollisionDispatcher *);        /* 0x04 */
    void *(*m_cleanupFunc)(hkpAgentEntry *, void *, hkpContactMgr *, hkpConstraintOwner *);                                        /* 0x08 */
    void (*m_removePointFunc)(hkpAgentEntry *, void *, unsigned __int16);                                                          /* 0x0C */
    void (*m_commitPotentialFunc)(hkpAgentEntry *, void *, unsigned __int16);                                                      /* 0x10 */
    void (*m_createZombieFunc)(hkpAgentEntry *, void *, unsigned __int16);                                                         /* 0x14 */
    void (*m_updateFilterFunc)(hkpAgentEntry *, void *, const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpContactMgr *, hkpConstraintOwner *); /* 0x18 */
    void (*m_calcStatisticsFunc)(hkpAgentEntry *, void *, const hkpCollisionInput *, hkStatisticsCollector *);                     /* 0x1C */
    void (*m_invalidateTimFunc)(hkpAgentEntry *, void *, const hkpCollisionInput *);                                               /* 0x20 */
    void (*m_warpTimeFunc)(hkpAgentEntry *, void *, float, float, const hkpCollisionInput *);                                      /* 0x24 */
    void (*m_sepNormalFunc)(const hkpAgent3Input *, hkpAgentEntry *, void *, hkVector4 *);                                         /* 0x28 */
    void *(*m_processFunc)(const hkpAgent3ProcessInput *, hkpAgentEntry *, void *, hkVector4 *, hkpProcessCollisionOutput *);      /* 0x2C */
    hkBool m_isPredictive;          /* 0x30 */
    hkBool m_ignoreSymmetricVersion;/* 0x31 */
    hkBool m_reusePreviousEntry;    /* 0x32 */
    unsigned char _pad33[13];       /* 0x33 -> 0x40 */
} hkpCollisionDispatcher_Agent3Funcs;

/* types_members hkpCollisionDispatcher::Agent3FuncsIntern size 80 */
typedef hkpCollisionDispatcher_Agent3Funcs Agent3Funcs;
struct hkpCollisionDispatcher_Agent3FuncsIntern : Agent3Funcs
{
    Symmetric m_symmetric;              /* 0x40 hkAgent3::Symmetric */
    unsigned char _pad44[12];           /* 0x44 -> 0x50 */
};
typedef struct hkpCollisionDispatcher_Agent3FuncsIntern hkpCollisionDispatcher_Agent3FuncsIntern;

/* types_members hkpCollisionQualityInfo size 64 */
typedef struct hkpCollisionQualityInfo
{
    float m_keepContact;                   /* 0x00 */
    float m_create4dContact;               /* 0x04 */
    float m_createContact;                 /* 0x08 */
    float m_manifoldTimDistance;           /* 0x0C */
    hkPadSpu<int> m_useContinuousPhysics;  /* 0x10 */
    hkBool m_useSimpleToiHandling;         /* 0x14 */
    unsigned char _pad15[3];               /* 0x15 */
    float m_minSeparation;                 /* 0x18 */
    float m_minExtraSeparation;            /* 0x1C */
    float m_minSafeDeltaTime;              /* 0x20 */
    float m_minAbsoluteSafeDeltaTime;      /* 0x24 */
    float m_toiSeparation;                 /* 0x28 */
    float m_toiExtraSeparation;            /* 0x2C */
    float m_toiAccuracy;                   /* 0x30 */
    float m_maxContraintViolation;         /* 0x34 */
    float m_minToiDeltaTime;               /* 0x38 */
    unsigned __int16 m_constraintPriority; /* 0x3C */
    hkBool m_enableToiWeldRejection;       /* 0x3E */
} hkpCollisionQualityInfo;

/* types_members hkpCollisionDispatcher::ShapeInheritance size 8 */
typedef struct hkpCollisionDispatcher_ShapeInheritance
{
    hkpShapeType m_primaryType;   /* 0x0 */
    hkpShapeType m_alternateType; /* 0x4 */
} hkpCollisionDispatcher_ShapeInheritance;

/* Local aliases so member spellings match the DB's nested-qualified types. */
typedef hkpCollisionDispatcher_AgentFuncs AgentFuncs;
typedef hkpCollisionDispatcher_Agent3FuncsIntern Agent3FuncsIntern;
typedef hkpCollisionDispatcher_ShapeInheritance ShapeInheritance;
typedef struct hkpCollisionDispatcher_DebugEntry DebugEntry; /* hkpCollisionDispatcher::DebugEntry */

/* hkpCollisionDispatcher, size 7760. Full DB layout (types_members). */
struct hkpCollisionDispatcher : hkReferencedObject
{
    hkpCollisionAgent *(*m_defaultCollisionAgent)(const hkpCdBody *, const hkpCdBody *, const hkpCollisionInput *, hkpContactMgr *); /* 0x8 */
    hkpContactMgrFactory *m_contactMgrFactory[8][8];       /* 0xC    */
    unsigned char _pad10C[4];                              /* 0x10C  */
    unsigned int m_hasAlternateType[32];                   /* 0x110  */
    int m_numAgent2Types;                                  /* 0x190  */
    unsigned char _pad194[12];                             /* 0x194  */
    unsigned char m_agent2Types[32][32];                   /* 0x1A0  agent-type dispatch table */
    unsigned char m_agent2TypesPred[32][32];               /* 0x5A0  */
    AgentFuncs m_agent2Func[64];                           /* 0x9A0  */
    int m_numAgent3Types;                                  /* 0xEA0  */
    unsigned char _padEA4[12];                             /* 0xEA4  */
    unsigned char m_agent3Types[32][32];                   /* 0xEB0  */
    unsigned char m_agent3TypesPred[32][32];               /* 0x12B0 */
    Agent3FuncsIntern m_agent3Func[16];                    /* 0x16B0 */
    char m_collisionQualityTable[10][10];                  /* 0x1BB0 */
    unsigned char _pad1C14[12];                            /* 0x1C14 */
    hkpCollisionQualityInfo m_collisionQualityInfo[8];     /* 0x1C20 */
    hkBool m_collisionAgentRegistered;                     /* 0x1E20 */
    hkBool m_agent3Registered;                             /* 0x1E21 */
    hkBool m_midphaseAgent3Registered;                     /* 0x1E22 */
    hkBool m_checkEnabled;                                 /* 0x1E23 */
    hkArray<ShapeInheritance> m_shapeInheritance;          /* 0x1E24 hkArray<hkpCollisionDispatcher::ShapeInheritance> */
    DebugEntry (*m_debugAgent2Table)[32][32];              /* 0x1E30 */
    DebugEntry (*m_debugAgent2TablePred)[32][32];          /* 0x1E34 */
    DebugEntry (*m_debugAgent3Table)[32][32];              /* 0x1E38 */
    DebugEntry (*m_debugAgent3TablePred)[32][32];          /* 0x1E3C */
    float m_expectedMaxLinearVelocity;                     /* 0x1E40 */
    float m_expectedMinPsiDeltaTime;                       /* 0x1E44 */
};
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
