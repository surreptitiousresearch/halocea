#pragma once
#include <stdint.h>
#include "hkArray.h"
#include "hkReferencedObject.h"
#include "hkBool.h"
#include "hkEnum.h"
#include "hkPadSpu.h"
#include "hkVector4.h"
#include "hkpSimulationIsland.h" /* hkMultiThreadCheck */

typedef struct hkpBroadPhaseBorder hkpBroadPhaseBorder;
typedef struct hkpSimulation hkpSimulation;
typedef struct hkpRigidBody hkpRigidBody;
typedef struct hkpWorldMaintenanceMgr hkpWorldMaintenanceMgr;
typedef struct hkWorldMemoryAvailableWatchDog hkWorldMemoryAvailableWatchDog;
typedef struct hkpBroadPhase hkpBroadPhase;
typedef struct hkpKdTreeWorldManager hkpKdTreeWorldManager;
typedef struct hkpTypedBroadPhaseDispatcher hkpTypedBroadPhaseDispatcher;
typedef struct hkpPhantomBroadPhaseListener hkpPhantomBroadPhaseListener;
typedef struct hkpEntityEntityBroadPhaseListener hkpEntityEntityBroadPhaseListener;
typedef struct hkpBroadPhaseBorderListener hkpBroadPhaseBorderListener;
typedef struct hkpMtThreadStructure hkpMtThreadStructure;
typedef struct hkpProcessCollisionInput hkpProcessCollisionInput;
typedef struct hkpCollisionFilter hkpCollisionFilter;
typedef struct hkpCollisionDispatcher hkpCollisionDispatcher;
typedef struct hkpConvexListFilter hkpConvexListFilter;
typedef struct hkpWorldOperationQueue hkpWorldOperationQueue;
typedef struct hkpDebugInfoOnPendingOperationQueues hkpDebugInfoOnPendingOperationQueues;
typedef struct hkCriticalSection hkCriticalSection;
typedef struct hkpPhantom hkpPhantom;
typedef struct hkpActionListener hkpActionListener;
typedef struct hkpEntityListener hkpEntityListener;
typedef struct hkpPhantomListener hkpPhantomListener;
typedef struct hkpConstraintListener hkpConstraintListener;
typedef struct hkpWorldDeletionListener hkpWorldDeletionListener;
typedef struct hkpIslandActivationListener hkpIslandActivationListener;
typedef struct hkpWorldPostSimulationListener hkpWorldPostSimulationListener;
typedef struct hkpWorldPostIntegrateListener hkpWorldPostIntegrateListener;
typedef struct hkpWorldPostCollideListener hkpWorldPostCollideListener;
typedef struct hkpIslandPostIntegrateListener hkpIslandPostIntegrateListener;
typedef struct hkpIslandPostCollideListener hkpIslandPostCollideListener;
typedef struct hkpCollisionListener hkpCollisionListener;
typedef struct hkpContactImpulseLimitBreachedListener hkpContactImpulseLimitBreachedListener;
typedef struct hkdWorld hkdWorld;
typedef uint16_t hkHalf; /* 16-bit float storage */

/* types_members hkStepInfo (size 16) */
typedef struct hkStepInfo
{
    hkPadSpu<float> m_startTime;    /* 0x0 */
    hkPadSpu<float> m_endTime;      /* 0x4 */
    hkPadSpu<float> m_deltaTime;    /* 0x8 */
    hkPadSpu<float> m_invDeltaTime; /* 0xC */
} hkStepInfo;

/* types_members hkpSolverInfo::DeactivationInfo (size 28) */
typedef struct hkpSolverInfo_DeactivationInfo
{
    float m_linearVelocityThresholdInv;    /* 0x00 */
    float m_angularVelocityThresholdInv;   /* 0x04 */
    float m_slowObjectVelocityMultiplier;  /* 0x08 */
    float m_relativeSleepVelocityThreshold;/* 0x0C */
    float m_maxDistSqrd[2];                /* 0x10 */
    hkHalf m_maxRotSqrd[2];                /* 0x18 */
} hkpSolverInfo_DeactivationInfo;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkpSolverInfo_DeactivationInfo DeactivationInfo;

/* types_members hkpSolverInfo (size 304) */
typedef struct hkpSolverInfo
{
    float m_one;                                 /* 0x000 */
    float m_tau;                                 /* 0x004 */
    float m_damping;                             /* 0x008 */
    float m_frictionTau;                         /* 0x00C */
    hkVector4 m_globalAccelerationPerSubStep;    /* 0x010 */
    hkVector4 m_globalAccelerationPerStep;       /* 0x020 */
    hkVector4 m_integrateVelocityFactor;         /* 0x030 */
    hkVector4 m_invIntegrateVelocityFactor;      /* 0x040 */
    float m_dampDivTau;                          /* 0x050 */
    float m_tauDivDamp;                          /* 0x054 */
    float m_dampDivFrictionTau;                  /* 0x058 */
    float m_frictionTauDivDamp;                  /* 0x05C */
    float m_contactRestingVelocity;              /* 0x060 */
    DeactivationInfo m_deactivationInfo[6];      /* 0x064 hkpSolverInfo::DeactivationInfo */
    float m_deltaTime;                           /* 0x10C */
    float m_invDeltaTime;                        /* 0x110 */
    int m_numSteps;                              /* 0x114 */
    int m_numMicroSteps;                         /* 0x118 */
    float m_invNumMicroSteps;                    /* 0x11C */
    float m_invNumSteps;                         /* 0x120 */
    hkBool m_forceCoherentConstraintOrderingInSolver;         /* 0x124 */
    uint8_t m_deactivationNumInactiveFramesSelectFlag[2]; /* 0x125 */
    uint8_t m_deactivationIntegrateCounter;           /* 0x127 */
} hkpSolverInfo;

/* types_members hkpWorldDynamicsStepInfo (size 320) */
typedef struct hkpWorldDynamicsStepInfo
{
    hkStepInfo m_stepInfo;     /* 0x00 */
    hkpSolverInfo m_solverInfo;/* 0x10 */
} hkpWorldDynamicsStepInfo;

/* types_enum_values hkpWorldCinfo::SimulationType */
enum SimulationType
{
    SIMULATION_TYPE_INVALID = 0,
    SIMULATION_TYPE_DISCRETE = 1,
    SIMULATION_TYPE_CONTINUOUS = 2,
    SIMULATION_TYPE_MULTITHREADED = 3,
};

/* types_enum_values hkpWorldCinfo::ContactPointGeneration */
enum ContactPointGeneration
{
    CONTACT_POINT_ACCEPT_ALWAYS = 0,
    CONTACT_POINT_REJECT_DUBIOUS = 1,
    CONTACT_POINT_REJECT_MANY = 2,
};

/* hkpWorld, size 816. Full DB layout (types_members hkpWorld). */
struct hkpWorld : hkReferencedObject
{
    hkpSimulation *m_simulation;                                     /* 0x008 */
    unsigned char _pad0C[4];                                         /* 0x00C */
    hkVector4 m_gravity;                                             /* 0x010 */
    hkpSimulationIsland *m_fixedIsland;                              /* 0x020 */
    hkpRigidBody *m_fixedRigidBody;                                  /* 0x024 */
    hkArray<hkpSimulationIsland *> m_activeSimulationIslands;        /* 0x028 */
    hkArray<hkpSimulationIsland *> m_inactiveSimulationIslands;      /* 0x034 */
    hkArray<hkpSimulationIsland *> m_dirtySimulationIslands;         /* 0x040 */
    hkpWorldMaintenanceMgr *m_maintenanceMgr;                        /* 0x04C */
    hkWorldMemoryAvailableWatchDog *m_memoryWatchDog;                /* 0x050 */
    hkpBroadPhase *m_broadPhase;                                     /* 0x054 */
    hkpKdTreeWorldManager *m_kdTreeManager;                          /* 0x058 */
    hkBool m_autoUpdateKdTree;                                       /* 0x05C */
    unsigned char _pad5D[3];                                         /* 0x05D */
    hkpTypedBroadPhaseDispatcher *m_broadPhaseDispatcher;            /* 0x060 */
    hkpPhantomBroadPhaseListener *m_phantomBroadPhaseListener;       /* 0x064 */
    hkpEntityEntityBroadPhaseListener *m_entityEntityBroadPhaseListener; /* 0x068 */
    hkpBroadPhaseBorderListener *m_broadPhaseBorderListener;         /* 0x06C */
    hkpMtThreadStructure *m_multithreadedSimulationJobData;          /* 0x070 */
    hkpProcessCollisionInput *m_collisionInput;                      /* 0x074 */
    hkpCollisionFilter *m_collisionFilter;                           /* 0x078 */
    hkpCollisionDispatcher *m_collisionDispatcher;                   /* 0x07C */
    hkpConvexListFilter *m_convexListFilter;                         /* 0x080 */
    hkpWorldOperationQueue *m_pendingOperations;                     /* 0x084 */
    int m_pendingOperationsCount;                                    /* 0x088 */
    int m_pendingBodyOperationsCount;                                /* 0x08C */
    int m_criticalOperationsLockCount;                               /* 0x090 */
    int m_criticalOperationsLockCountForPhantoms;                    /* 0x094 */
    hkBool m_blockExecutingPendingOperations;                        /* 0x098 */
    hkBool m_criticalOperationsAllowed;                              /* 0x099 */
    unsigned char _pad9A[2];                                         /* 0x09A */
    hkpDebugInfoOnPendingOperationQueues *m_pendingOperationQueues;  /* 0x09C */
    int m_pendingOperationQueueCount;                                /* 0x0A0 */
    hkMultiThreadCheck m_multiThreadCheck;                           /* 0x0A4 */
    hkBool m_processActionsInSingleThread;                           /* 0x0AC */
    unsigned char _padAD[3];                                         /* 0x0AD */
    unsigned int m_minDesiredIslandSize;                             /* 0x0B0 */
    hkCriticalSection *m_modifyConstraintCriticalSection;            /* 0x0B4 */
    int m_isLocked;                                                  /* 0x0B8 */
    hkCriticalSection *m_islandDirtyListCriticalSection;             /* 0x0BC */
    hkCriticalSection *m_propertyMasterLock;                         /* 0x0C0 */
    hkBool m_wantSimulationIslands;                                  /* 0x0C4 */
    unsigned char _padC5[3];                                         /* 0x0C5 */
    float m_snapCollisionToConvexEdgeThreshold;                      /* 0x0C8 */
    float m_snapCollisionToConcaveEdgeThreshold;                     /* 0x0CC */
    hkBool m_enableToiWeldRejection;                                 /* 0x0D0 */
    hkBool m_wantDeactivation;                                       /* 0x0D1 */
    hkBool m_shouldActivateOnRigidBodyTransformChange;               /* 0x0D2 */
    unsigned char _padD3;                                            /* 0x0D3 */
    float m_deactivationReferenceDistance;                           /* 0x0D4 */
    float m_toiCollisionResponseRotateNormal;                        /* 0x0D8 */
    int m_maxSectorsPerCollideTask;                                  /* 0x0DC */
    int m_maxEntriesPerToiCollideTask;                               /* 0x0E0 */
    hkEnum<SimulationType, int> m_simulationType;                    /* 0x0E4 hkpWorldCinfo::SimulationType */
    float m_numToisTillAllowedPenetrationSimplifiedToi;              /* 0x0E8 */
    float m_numToisTillAllowedPenetrationToi;                        /* 0x0EC */
    float m_numToisTillAllowedPenetrationToiHigher;                  /* 0x0F0 */
    float m_numToisTillAllowedPenetrationToiForced;                  /* 0x0F4 */
    unsigned int m_lastEntityUid;                                    /* 0x0F8 */
    hkArray<hkpPhantom *> m_phantoms;                                /* 0x0FC */
    hkArray<hkpActionListener *> m_actionListeners;                  /* 0x108 */
    hkArray<hkpEntityListener *> m_entityListeners;                  /* 0x114 */
    hkArray<hkpPhantomListener *> m_phantomListeners;                /* 0x120 */
    hkArray<hkpConstraintListener *> m_constraintListeners;          /* 0x12C */
    hkArray<hkpWorldDeletionListener *> m_worldDeletionListeners;    /* 0x138 */
    hkArray<hkpIslandActivationListener *> m_islandActivationListeners; /* 0x144 */
    hkArray<hkpWorldPostSimulationListener *> m_worldPostSimulationListeners; /* 0x150 */
    hkArray<hkpWorldPostIntegrateListener *> m_worldPostIntegrateListeners; /* 0x15C */
    hkArray<hkpWorldPostCollideListener *> m_worldPostCollideListeners; /* 0x168 */
    hkArray<hkpIslandPostIntegrateListener *> m_islandPostIntegrateListeners; /* 0x174 */
    hkArray<hkpIslandPostCollideListener *> m_islandPostCollideListeners; /* 0x180 */
    hkArray<hkpCollisionListener *> m_collisionListeners;            /* 0x18C */
    hkArray<hkpContactImpulseLimitBreachedListener *> m_contactImpulseLimitBreachedListeners; /* 0x198 */
    hkpBroadPhaseBorder *m_broadPhaseBorder;                         /* 0x1A4 */
    hkdWorld *m_destructionWorld;                                    /* 0x1A8 */
    unsigned char _pad1AC[4];                                        /* 0x1AC */
    hkpWorldDynamicsStepInfo m_dynamicsStepInfo;                     /* 0x1B0 (320) */
    hkVector4 m_broadPhaseExtents[2];                                /* 0x2F0 */
    int m_broadPhaseNumMarkers;                                      /* 0x310 */
    int m_sizeOfToiEventQueue;                                       /* 0x314 */
    int m_broadPhaseQuerySize;                                       /* 0x318 */
    int m_broadPhaseUpdateSize;                                      /* 0x31C */
    hkEnum<ContactPointGeneration, signed char> m_contactPointGeneration; /* 0x320 hkpWorldCinfo::ContactPointGeneration */
};
typedef struct hkpWorld hkpWorld;
