#pragma once
#include <stdint.h>
#include "hkArray.h"
#include "hkReferencedObject.h"
#include "hkInplaceArray.h"

typedef struct hkpWorld hkpWorld;
typedef struct hkpAction hkpAction;
typedef struct hkpEntity hkpEntity;
typedef struct hkpAgentNnSector hkpAgentNnSector;

/* types_members hkpConstraintInfo (size 16) */
typedef struct hkpConstraintInfo
{
    int m_maxSizeOfSchema;    /* 0x0 */
    int m_sizeOfSchemas;      /* 0x4 */
    int m_numSolverResults;   /* 0x8 */
    int m_numSolverElemTemps; /* 0xC */
} hkpConstraintInfo;

/* types_members hkpConstraintOwner (size 24) */
struct hkpConstraintOwner : hkReferencedObject
{
    hkpConstraintInfo m_constraintInfo; /* 0x8 */
};
typedef struct hkpConstraintOwner hkpConstraintOwner;

/* types_members hkMultiThreadCheck (size 8) */
#include "hkMultiThreadCheck.h"

/* types_members hkpAgentNnTrack (size 20) */
typedef struct hkpAgentNnTrack
{
    unsigned int m_bytesUsedInLastSector;             /* 0x0 */
    hkInplaceArray<hkpAgentNnSector *, 1> m_sectors;  /* 0x4 */
} hkpAgentNnTrack;

/* types_members hkpSimulationIsland (size 108) */
struct hkpSimulationIsland : hkpConstraintOwner
{
    hkpWorld *m_world;                          /* 0x18 */
    int m_numConstraints;                       /* 0x1C */
    uint16_t m_storageIndex;            /* 0x20 */
    uint16_t m_dirtyListIndex;          /* 0x22 */
    uint8_t m_splitCheckFrameCounter;   /* 0x24 */
    int8_t m_allowIslandLocking : 2;            /* 0x25 */
    int8_t m_actionListCleanupNeeded : 2;
    int8_t m_sparseEnabled : 2;
    int8_t m_splitCheckRequested : 2;
    uint8_t _reserved_bits : 2;         /* 0x26 (anonymous in DB) */
    int8_t m_inIntegrateJob : 2;
    int8_t m_activeMark : 2;
    int8_t m_isInActiveIslandsArray : 2;
    unsigned char _pad27;                       /* 0x27 */
    hkMultiThreadCheck m_multiThreadCheck;      /* 0x28 */
    float m_timeSinceLastHighFrequencyCheck;    /* 0x30 */
    float m_timeSinceLastLowFrequencyCheck;     /* 0x34 */
    hkArray<hkpAction *> m_actions;             /* 0x38 */
    float m_timeOfDeactivation;                 /* 0x44 */
    hkInplaceArray<hkpEntity *, 1> m_entities;  /* 0x48 */
    hkpAgentNnTrack m_agentTrack;               /* 0x58 -> 108 */
};
typedef struct hkpSimulationIsland hkpSimulationIsland;
