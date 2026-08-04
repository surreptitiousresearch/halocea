#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsSTRID_FLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../m3d/m3dSPL_LINEAR1D.h"
#include "../m3d/m3dRND_EXCLUSIVE.h"
#include "../ds/dsTYPE_ID.h"
#include "../ps/psSECTION.h"
#include "aiSENSES.h"
#include "../fsm/fsmFLOAT_ARRAY.h"
#include "aiGOAL_BASE.h" // aiSPD
#include "../DEFAULT_CTOR.h"
#include "../sml/smlSTATE_ID.h"
#include "../ent/entDESC.h"
// ws-engine ai08: the descriptor (data-driven parameter block) an aiBRAIN is built from.
// Full DB-verified layout (types_members aiBRAIN_DESC) — size 0x670, rooted at entDESC.
// The entDESC base is typed (entDESC.h, size 0x1B8); every field the parser methods read/write is
// materialised at its DB offset with its DB type. Methods are declared non-virtual (the vtable pointer lives inside the entDESC base span),
// matching the aiBRAIN.h sibling convention so the data layout stays exact. Offsets are DB-verified.

struct psSECTION_INTERFACE;
enum AI_MORAL_TYPE {        // DB types_enum_values(AI_MORAL_TYPE)
    AI_MORAL_COWARD  = 0,
    AI_MORAL_COMMON  = 1,
    AI_MORAL_BRAVERY = 2,
};

struct aiBRAIN_DESC : entDESC { // base 0x000..0x1B8 (DB-typed)
    // Which subsystem-set variant a given AI subsystem uses. DB enum aiBRAIN_DESC::aiSUBSYS_TYPE.
    enum aiSUBSYS_TYPE {
        aiSUBSYS_NULL   = 1,
        aiSUBSYS_IDLE   = 2,
        aiSUBSYS_BATTLE = 3,
        aiSUBSYS_FULL   = 4,
        aiSUBSYS_LAST   = 5,
    };

    // DB types_members aiBRAIN_DESC::ABILITIES — 27 per-unit ability toggles (size 0x1B).
    struct ABILITIES {
        bool HO;                       // 0x00
        bool NS;                       // 0x01
        bool relocalize;               // 0x02
        bool minizoom;                 // 0x03
        bool blindShoot;               // 0x04
        bool floorPlace;               // 0x05
        bool visionReal;               // 0x06
        bool reload;                   // 0x07
        bool is3DMove;                 // 0x08
        bool melee;                    // 0x09
        bool miniGameMelee;            // 0x0A
        bool optimizable;              // 0x0B
        bool boss;                     // 0x0C
        bool reactToReticle;           // 0x0D
        bool aimAtNeverSeenEnemy;      // 0x0E
        bool shootAtIdleEnemy;         // 0x0F
        bool throwGrenadeAtOpenEnemy;  // 0x10
        bool throwGrenadeAtPlayerOnly; // 0x11
        bool grenadeAvoidance;         // 0x12
        bool physObjAvoidance;         // 0x13
        bool acceptFriendlyFire;       // 0x14
        bool changeEnemyDuringStrike;  // 0x15
        bool checkVisForBlindShoot;    // 0x16
        bool tinyDamageFromPlayer;     // 0x17
        bool smartObjects;             // 0x18
        bool jumpOverHO;               // 0x19
        bool rollBetweenHO;            // 0x1A
    };

    // A single perception descriptor (idle / battle senses). DB: aiBRAIN_DESC::aiSENSES_DESC : aiSENSES.
    struct aiSENSES_DESC : aiSENSES {
        // 0x83185B90 (?ParsePS@aiSENSES_DESC@aiBRAIN_DESC@@QAAXABVpsSECTION@@@Z) — read sight/hearing
        // ranges from `ps`, clamping the side cone/range to be no smaller than the straight ones.
        // REVERSED: aiBRAIN_DESC__aiSENSES_DESC__ParsePS.cpp.
        void ParsePS(const psSECTION &ps);
    };

    // Nav dead-reckoning sub-block. DB: aiBRAIN_DESC::DNS.
    struct DNS {
        int   nPoints; // 0x00
        float distMin; // 0x04
        float distMax; // 0x08
    };

    // 0x843223EC (?TYPE_ID@aiBRAIN_DESC@@2VdsTYPE_ID@@A) — static RTTI id of the aiBRAIN_DESC family.
    static dsTYPE_ID TYPE_ID;

    aiSUBSYS_TYPE ssTypeBody;                // 0x1B8
    aiSUBSYS_TYPE ssTypeAim;                 // 0x1BC
    aiSUBSYS_TYPE ssTypeAprog;               // 0x1C0
    aiSUBSYS_TYPE ssTypeBest;                // 0x1C4
    aiSUBSYS_TYPE ssTypeEnemy;               // 0x1C8
    aiSUBSYS_TYPE ssTypeIsa;                 // 0x1CC
    aiSUBSYS_TYPE ssTypeGoal;                // 0x1D0
    aiSUBSYS_TYPE ssTypePerc;                // 0x1D4
    aiSUBSYS_TYPE ssTypeVision;              // 0x1D8
    aiSUBSYS_TYPE ssTypeSelene;              // 0x1DC
    aiSUBSYS_TYPE ssTypeSnd;                 // 0x1E0
    aiSUBSYS_TYPE ssTypeTask;                // 0x1E4
    ABILITIES     abil;                      // 0x1E8 per-unit ability toggles (0x1B)

    bool          isClipUpAngle;             // 0x203
    bool          isClipDownAngle;           // 0x204
    unsigned char _pad205[3];                // 0x205
    float         timeUpdatePeerOnLF;        // 0x208
    float         timeLockEnemy;             // 0x20C
    aiSENSES_DESC idle;                      // 0x210
    aiSENSES_DESC battle;                    // 0x22C
    fsmFLOAT_ARRAY grAvoidRadius;            // 0x248
    fsmFLOAT_ARRAY grAvoidDelay;             // 0x254
    float         minEnemyDamaged;           // 0x260
    float         speedEnemyDamagedDec;      // 0x264
    float         injDmgToChangeEnemy;       // 0x268
    float         injTimeLockEnemy;          // 0x26C
    float         injTimeDisable;            // 0x270
    float         injWndDecreaseSpeed;       // 0x274
    dsSTRID_FLAGS sfilterImmune;             // 0x278
    dsVECTOR<aiSPD, 8> speedList;            // 0x28C
    dsSTRID       stayFsmEvent;              // 0x2A0
    dsSTRID       tauntFsmEvent;             // 0x2A4
    bool          mayShootInvisibleEnemy;    // 0x2A8
    unsigned char _pad2A9[3];                // 0x2A9
    float         radAfraidGrenade;          // 0x2AC
    float         towerManStrength;          // 0x2B0
    bool          isSmtDeathEnabled;         // 0x2B4
    unsigned char _pad2B5[3];                // 0x2B5
    float         smtDeathMaxDist;           // 0x2B8
    bool          avoid3DHOPaths;            // 0x2BC
    bool          enableHOOn3DHOPath;        // 0x2BD
    bool          disable3DHOMeleePoints;    // 0x2BE
    bool          force3DHONearEnemy;        // 0x2BF
    float         force3DHODist;             // 0x2C0
    float         dotProdPathBigTurn;        // 0x2C4
    bool          alwaysInHO;                // 0x2C8
    unsigned char _pad2C9[3];                // 0x2C9
    float         distReachMultiplier;       // 0x2CC scale applied to a goal's reach distance
    float         distReachLastPoint;        // 0x2D0
    float         timeYieldWay;              // 0x2D4
    float         distYieldWay;              // 0x2D8
    dsSTRID       speedMelee;                // 0x2DC
    unsigned short weakFlagsDisabledEdges;   // 0x2E0
    unsigned char _pad2E2[2];                // 0x2E2
    unsigned int  weakFlagsBlockedWP;        // 0x2E4
    DNS           dns;                       // 0x2E8

    float         distEnemyPlayerOnly;       // 0x2F4
    int           maxMeleeEnemies;           // 0x2F8
    int           maxRangedEnemies;          // 0x2FC
    float         coefFER_dist_PlrOpened;    // 0x300
    float         coefFER_dist_PlrCovered;   // 0x304
    float         coefFER_dist_AiOpened;     // 0x308
    float         coefFER_dist_AiCovered;    // 0x30C
    float         coefFEM_dist_Plr;          // 0x310
    float         coefFEM_dist_AI;           // 0x314
    float         shiftQuarterOrigDir;       // 0x318
    float         shiftQuarterOrigUp;        // 0x31C
    float         shiftQuarterOrigRight;     // 0x320
    AI_MORAL_TYPE moral;                     // 0x324
    float         shiftVisionPosInHO;        // 0x328
    float         shiftPosFaceHorInHighHO;   // 0x32C
    float         shiftPosFaceVertInHighHO;  // 0x330
    m3dSPL_LINEAR1D shiftAimPosInLowHO;      // 0x334
    m3dSPL_LINEAR1D curveSndPitch;           // 0x348
    bool          forceSetupInitial;         // 0x35C
    unsigned char _pad35D[3];                // 0x35D
    int           idxOnScreen;               // 0x360
    dsCONST_ARRAY<float, 16> navEdgePeerWeight;      // 0x364
    float         enterHOPenaltyCenterShiftToHO;     // 0x3A8
    float         enterHOPenaltyCenterShiftToCorner; // 0x3AC
    float         enterHOPosShiftToHO;       // 0x3B0
    float         enterHOPosShiftToCorner;   // 0x3B4
    unsigned char _navWPForbidFlags[4];      // 0x3B8 apSTATE_T<unsigned long> (opaque)
    float         navAssocDistance;          // 0x3BC
    float         distFlyObjCollision_2;     // 0x3C0
    dsVECTOR<dsSTRID, 8> reactFlyObj_Coll_Right; // 0x3C4
    dsVECTOR<dsSTRID, 8> reactFlyObj_Coll_Left;  // 0x3D8
    float         distFlyObjAfraid_2;        // 0x3EC
    dsVECTOR<dsSTRID, 8> reactFlyObj_Afr_Right;  // 0x3F0
    dsVECTOR<dsSTRID, 8> reactFlyObj_Afr_Left;   // 0x404
    dsTSTRING<char> voiceType;               // 0x418
    int           voiceCount;                // 0x41C
    m3dRND_EXCLUSIVE voiceRnd;               // 0x420
    dsSTRID       voiceTeam;                 // 0x428
    psSECTION     psApproach;                // 0x42C
    psSECTION     psBehaviors;               // 0x430

    // ---- SSL class-registration id tail (DB types_members aiBRAIN_DESC, 0x434..0x670) ----------
    // Materialized from the DB: the once-registered SSL event ids (RegisterEvent) and callback
    // function ids (AddCbFunc) filled in by InitSslClass, then the __sml* state-id tail.
    DEFAULT_CTOR<bool>   isSslClassInited;                        // 0x434
    int                  sslEventId_OnBhvEnd;                     // 0x438
    int                  sslEventId_OnBhvEndFailed;               // 0x43C
    int                  sslEventId_OnBhvEndSuccess;              // 0x440
    int                  sslEventId_OnBhvEndTerminate;            // 0x444
    int                  sslEventId_OnAlert;                      // 0x448
    int                  sslEventId_OnReload;                     // 0x44C
    int                  sslEventId_OnEnemySeen;                  // 0x450
    int                  sslEventId_OnInit;                       // 0x454
    int                  sslEventId_OnWounded;                    // 0x458
    int                  sslEventId_OnComplete;                   // 0x45C
    int                  sslEventId_OnKilledByPlayer;             // 0x460
    int                  sslEventId_OnBattleChanged;              // 0x464
    int                  sslEventId_OnEnter;                      // 0x468
    int                  sslEventId_OnLeave;                      // 0x46C
    int                  sslEventId_OnReach;                      // 0x470
    int                  sslEventId_OnEnterHO;                    // 0x474
    int                  sslEventId_OnLeaveHO;                    // 0x478
    int                  sslEventId_OnStrikeStart;                // 0x47C
    int                  sslEventId_OnStrikeEnd;                  // 0x480
    int                  sslEventId_OnEnemyDamaged;               // 0x484
    int                  sslFuncId_IsSpawnedFrom;                 // 0x488
    int                  sslFuncId_OnCreate;                      // 0x48C
    int                  sslFuncId_OnDestroy;                     // 0x490
    int                  sslFuncId_OnFrameBegin;                  // 0x494
    int                  sslFuncId_OnDbgOut;                      // 0x498
    int                  sslFuncId_OnFrame;                       // 0x49C
    int                  sslFuncId_Terminate;                     // 0x4A0
    int                  sslFuncId_DropWeapon;                    // 0x4A4
    int                  sslFuncId_SetEnemy;                      // 0x4A8
    int                  sslFuncId_EndEnemy;                      // 0x4AC
    int                  sslFuncId_EndEnemySoft;                  // 0x4B0
    int                  sslFuncId_GetEnemy;                      // 0x4B4
    int                  sslFuncId_IsEnemyType;                   // 0x4B8
    int                  sslFuncId_SetAim;                        // 0x4BC
    int                  sslFuncId_EndAim;                        // 0x4C0
    int                  sslFuncId_GetAim;                        // 0x4C4
    int                  sslFuncId_SetSenses;                     // 0x4C8
    int                  sslFuncId_Alert;                         // 0x4CC
    int                  sslFuncId_ResetAlerts;                   // 0x4D0
    int                  sslFuncId_SetSniperVision;               // 0x4D4
    int                  sslFuncId_SetApproach;                   // 0x4D8
    int                  sslFuncId_EndApproach;                   // 0x4DC
    int                  sslFuncId_ForceWar;                      // 0x4E0
    int                  sslFuncId_ForcePeace;                    // 0x4E4
    int                  sslFuncId_AutoBattle;                    // 0x4E8
    int                  sslFuncId_ShootNow;                      // 0x4EC
    int                  sslFuncId_EnableStrike;                  // 0x4F0
    int                  sslFuncId_DisableStrike;                 // 0x4F4
    int                  sslFuncId_LockStrike;                    // 0x4F8
    int                  sslFuncId_CancelCurStrike;               // 0x4FC
    int                  sslFuncId_DbgStrike;                     // 0x500
    int                  sslFuncId_SetWpn;                        // 0x504
    int                  sslFuncId_SetSpreadAngle;                // 0x508
    int                  sslFuncId_SetDamageCurve;                // 0x50C
    int                  sslFuncId_AIGetCurWeaponClassName;       // 0x510
    int                  sslFuncId_SetContourShootingDist;        // 0x514
    int                  sslFuncId_HasAnyAvailableStrike;         // 0x518
    int                  sslFuncId_IsLive;                        // 0x51C
    int                  sslFuncId_SetPar;                        // 0x520
    int                  sslFuncId_GetFloat;                      // 0x524
    int                  sslFuncId_SetFloat;                      // 0x528
    int                  sslFuncId_GetBool;                       // 0x52C
    int                  sslFuncId_GetInt;                        // 0x530
    int                  sslFuncId_GetString;                     // 0x534
    int                  sslFuncId_GetNameSpawnedFrom;            // 0x538
    int                  sslFuncId_SetViewUpCurve;                // 0x53C
    int                  sslFuncId_CalcReticleDanger;             // 0x540
    int                  sslFuncId_SetTimer;                      // 0x544
    int                  sslFuncId_KillTimer;                     // 0x548
    int                  sslFuncId_OnTimer;                       // 0x54C
    int                  sslFuncId_EnemyResetMovementDist;        // 0x550
    int                  sslFuncId_EnemyGetMovementDist;          // 0x554
    int                  sslFuncId_ThrowWeaponsAndItems;          // 0x558
    int                  sslFuncId_LockEnemy;                     // 0x55C
    int                  sslFuncId_SpareEnemy;                    // 0x560
    int                  sslFuncId_Exclaim;                       // 0x564
    int                  sslFuncId_OnExStarted;                   // 0x568
    int                  sslFuncId_OnExPlayed;                    // 0x56C
    int                  sslFuncId_OnExCancelled;                 // 0x570
    int                  sslFuncId_FindExPartner;                 // 0x574
    int                  sslFuncId_SetNavSys;                     // 0x578
    int                  sslFuncId_GetNavSys;                     // 0x57C
    int                  sslFuncId_MoveToPoint;                   // 0x580
    int                  sslFuncId_MoveToHO;                      // 0x584
    int                  sslFuncId_MoveToNavSys;                  // 0x588
    int                  sslFuncId_HasHOInNavSys;                 // 0x58C
    int                  sslFuncId_ForgetStuck;                   // 0x590
    int                  sslFuncId_GetDistToObj;                  // 0x594
    int                  sslFuncId_GetDistToWP;                   // 0x598
    int                  sslFuncId_GetClosestPoint;               // 0x59C
    int                  sslFuncId_GetPathRemain;                 // 0x5A0
    int                  sslFuncId_GetDistPlayerToPathEnd;        // 0x5A4
    int                  sslFuncId_SetForceSpeed;                 // 0x5A8
    int                  sslFuncId_EndForceSpeed;                 // 0x5AC
    int                  sslFuncId_UseSmartObject;                // 0x5B0
    int                  sslFuncId_UseSmartObjectInstant;         // 0x5B4
    int                  sslFuncId_AttachBodyToSmtObj;            // 0x5B8
    int                  sslFuncId_UseTurret;                     // 0x5BC
    int                  sslFuncId_FindBestTurret;                // 0x5C0
    int                  sslFuncId_UseSmartIdleInstant;           // 0x5C4
    int                  sslFuncId_SetBHV;                        // 0x5C8
    int                  sslFuncId_PushBHV;                       // 0x5CC
    int                  sslFuncId_PushSHOOT;                     // 0x5D0
    int                  sslFuncId_GetBHV;                        // 0x5D4
    int                  sslFuncId_GetMajorBHVType;               // 0x5D8
    int                  sslFuncId_SetBHVOnEmptyStack;            // 0x5DC
    int                  sslFuncId_CallBhvFuncOnce;               // 0x5E0
    int                  sslFuncId_CancelBHVByNameAndStackAbove;  // 0x5E4
    int                  sslFuncId_FailBHVByNameAndStackAbove;    // 0x5E8
    int                  sslFuncId_JoinGroup;                     // 0x5EC
    int                  sslFuncId_LeaveGroup;                    // 0x5F0
    int                  sslFuncId_HaveGroup;                     // 0x5F4
    int                  sslFuncId_GetGroup;                      // 0x5F8
    int                  sslFuncId_SetInterest;                   // 0x5FC
    int                  sslFuncId_EndInterest;                   // 0x600
    int                  sslFuncId_GetTeamName;                   // 0x604
    int                  sslFuncId_SetTeamName;                   // 0x608
    int                  sslFuncId_CountMyNeutralsAroundMe;       // 0x60C
    int                  sslFuncId_CountMyNeutralsAroundEnemy;    // 0x610
    int                  sslFuncId_tDo;                           // 0x614
    int                  sslFuncId_tDoWait;                       // 0x618
    int                  sslFuncId_tNDo;                          // 0x61C
    int                  sslFuncId_tNDoWait;                      // 0x620
    int                  sslFuncId_tClear;                        // 0x624
    int                  sslFuncId_tWaitBHV;                      // 0x628
    int                  sslFuncId_tWait;                         // 0x62C
    int                  sslFuncId_tLock;                         // 0x630
    int                  sslFuncId_tUnlock;                       // 0x634
    int                  sslFuncId_tSetAFCount;                   // 0x638
    int                  sslFuncId_tSetAFDelay;                   // 0x63C
    int                  sslFuncId_tEnableForceAF;                // 0x640
    int                  sslFuncId_TestParam2AI;                  // 0x644
    int                  sslFuncId_TestParamFromAI;               // 0x648
    int                  sslFuncId_QOT_Reset;                     // 0x64C
    int                  sslFuncId_QOT_Put;                       // 0x650
    int                  sslFuncId_QOT_GetFront;                  // 0x654
    int                  sslFuncId_QOT_ClearFront;                // 0x658
    int                  sslFuncId_QOT_Size;                      // 0x65C
    int                  sslFuncId_QOT_IsEmpty;                   // 0x660
    int                  sslFuncId_AvoidDanger;                   // 0x664
    int                  sslFuncId_AvoidGrenade;                  // 0x668
    smlSTATE_ID          __smlState_CDT_HAPPENED;                 // 0x66C

    // ---- reversed in this batch ------------------------------------------------------------
    // 0x83186C90 (?ParsePS@aiBRAIN_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — parse a brain
    // descriptor from `psRead`: entDESC base fields, priority, ReadAll, then the nested "AI" section.
    // REVERSED: aiBRAIN_DESC__ParsePS.cpp.
    void ParsePS(psSECTION psRead, const dsTSTRING<char> &hintErr);
    // 0x83186AF0 (?GetVoiceType@aiBRAIN_DESC@@QAAABV?$dsTSTRING@D@@XZ) — pick this class's voice type
    // string (randomizing "<voiceType><n>" when voiceCount>1). REVERSED: aiBRAIN_DESC__GetVoiceType.cpp.
    const dsTSTRING<char> &GetVoiceType();

    // 0x83185E08 (?GetVoiceTeam@aiBRAIN_DESC@@QAAABVdsSTRID@@XZ) — this class's exclamation/voice team
    // id (returned by reference). boundary.
    const dsSTRID &GetVoiceTeam();
    // 0x83185CC0 (?ReadOther@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadOther.cpp.
    void ReadOther(const psSECTION &psRead);
    // 0x83185D10 (?ReadAfraid@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadAfraid.cpp.
    void ReadAfraid(const psSECTION &psRead);
    // 0x83185E10 (?ReadHealthDeath@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadHealthDeath.cpp.
    void ReadHealthDeath(const psSECTION &psRead);
    // 0x83185F18 (?ReadSFX@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadSFX.cpp.
    void ReadSFX(const psSECTION &psRead);
    // 0x83185F90 (?ReadSaveSections@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadSaveSections.cpp.
    void ReadSaveSections(const psSECTION &psRead);
    // 0x83186310 (?ReadNav@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadNav.cpp.
    void ReadNav(const psSECTION &psRead);
    // 0x83186490 (?ReadBehavModifiers@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadBehavModifiers.cpp.
    void ReadBehavModifiers(const psSECTION &psRead);
    // 0x83186D78 (?ReadPerception@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z). REVERSED: aiBRAIN_DESC__ReadPerception.cpp.
    void ReadPerception(const psSECTION &psRead);

    // 0x83185788 (?ReadAbilities@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z) — read the "abilities"
    // subsection into `abil`. REVERSED: aiBRAIN_DESC__ReadAbilities.cpp.
    void ReadAbilities(const psSECTION &psRead);
    // No-arg subsystem-type read (vtable slot 0x2C), dispatched by ReadAll. body external. boundary.
    void ReadSubsystems();
    // 0x83188618 (?ReadSubsystems@aiBRAIN_DESC@@MAAXABVpsSECTION@@@Z) — read the "subsystems"
    // subsection into the ssType* fields. REVERSED: aiBRAIN_DESC__ReadSubsystems.cpp.
    void ReadSubsystems(const psSECTION &psRead);
    // 0x83188910 (?ReadSpeed@aiBRAIN_DESC@@MAAXABVpsSECTION@@ABV?$dsTSTRING@D@@1@Z) — parse one
    // named speed entry into speedList. REVERSED: aiBRAIN_DESC__ReadSpeed.cpp.
    void ReadSpeed(const psSECTION &psRead, const dsTSTRING<char> &name,
                   const dsTSTRING<char> &hintErr);
    // 0x83187A98 — read the "speedList" subsection. body external. boundary.
    void ReadSpeedList(const psSECTION &psRead, const dsTSTRING<char> &hintErr);
    // 0x83186F30 — read the voice block. body external. boundary.
    void ReadVoices(const psSECTION &psRead);
    // 0x83187420 — read the enemy-selection block. body external. boundary.
    void ReadEnemy(const psSECTION &psRead);

    // 0x83185538 (?ReadAll@aiBRAIN_DESC@@MAAXABVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — fan a descriptor
    // section out to every Read* sub-parser in order. REVERSED: aiBRAIN_DESC__ReadAll.cpp.
    void ReadAll(const psSECTION &psRead, const dsTSTRING<char> &hintErr);

    // 0x8325BCB8 (?InitSslClass@aiBRAIN_DESC@@UAAHABV?$dsTSTRING@D@@@Z) — one-time SSL-class setup:
    // chain to entDESC::InitSslClass, then (guarded by isSslClassInited) unshare this class's
    // sslClass and register every brain SSL event + native callback function on it, recording each
    // id into the sslEventId_/sslFuncId_ tail. REVERSED: aiBRAIN_DESC__InitSslClass.cpp.
    int InitSslClass(const dsTSTRING<char> &hintErr);
    // 0x8325xxxx — unshare (copy-on-write) this descriptor's SSL class ref before registering onto it.
    // body external — next frontier. boundary.
    void UnshareSslClass(bool a, bool b);
};
