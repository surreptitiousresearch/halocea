#pragma once
// Boundary accessors into aiBRAIN's owned species descriptor (spDesc.pointee) for the aiBRAIN "cmd"
// SSL-function batch (0x83249A50-0x8324A5C8). The descriptor's dynamic type is an aiBRAIN_DESC (the
// NEXT FRONTIER); its static type is dscDESC. Each of these methods reads one per-species SSL
// *function* id (a 4-byte word) out of the descriptor and dispatches it via sslObject.CallFunc.
//
// The decompiler expresses each read as `spDesc.pointee[N].<field>` over a dscDESC[] view
// (sizeof(dscDESC) == 12; __vftable@0, ref@4, pBrand@8); the absolute byte offsets below are
// N*12 + field-offset, matching the disassembly at each site. Rather than fabricate the full
// aiBRAIN_DESC layout, these read the exact word each site reads.

struct dscDESC;

static inline int aiBRAIN_DESC_SslFuncId(const dscDESC *desc, unsigned int byteOffset) {
    return *reinterpret_cast<const int *>(
        reinterpret_cast<const unsigned char *>(desc) + byteOffset);
}

// spDesc.pointee[100].pBrand   == byte offset 1208 — "IsEnemyType"
static inline int aiBRAIN_DESC_SslFnIsEnemyType(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1208); }
// spDesc.pointee[101].__vftable == byte offset 1212 — "SetAim"
static inline int aiBRAIN_DESC_SslFnSetAim(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1212); }
// spDesc.pointee[101].ref       == byte offset 1216 — "EndAim"
static inline int aiBRAIN_DESC_SslFnEndAim(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1216); }
// spDesc.pointee[102].__vftable == byte offset 1224 — "SetSenses"
static inline int aiBRAIN_DESC_SslFnSetSenses(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1224); }
// spDesc.pointee[102].pBrand    == byte offset 1232 — "ResetAlerts"
static inline int aiBRAIN_DESC_SslFnResetAlerts(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1232); }
// spDesc.pointee[103].__vftable == byte offset 1236 — "SetSniperVision"
static inline int aiBRAIN_DESC_SslFnSetSniperVision(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1236); }
// spDesc.pointee[104].__vftable == byte offset 1248 — "ForceWar"
static inline int aiBRAIN_DESC_SslFnForceWar(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1248); }
// spDesc.pointee[104].ref       == byte offset 1252 — "ForcePeace"
static inline int aiBRAIN_DESC_SslFnForcePeace(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1252); }
// spDesc.pointee[104].pBrand    == byte offset 1256 — "AutoBattle"
static inline int aiBRAIN_DESC_SslFnAutoBattle(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1256); }
// spDesc.pointee[105].__vftable == byte offset 1260 — "ShootNow"
static inline int aiBRAIN_DESC_SslFnShootNow(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1260); }
// spDesc.pointee[105].ref       == byte offset 1264 — "EnableStrike"
static inline int aiBRAIN_DESC_SslFnEnableStrike(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1264); }
// spDesc.pointee[109].ref       == byte offset 1312 — "SetPar"
static inline int aiBRAIN_DESC_SslFnSetPar(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1312); }

// --- aiBRAIN strike/weapon "cmd" batch (0x8324A708-0x8324B370), same descriptor image ------------
// spDesc.pointee[105].pBrand    == byte offset 1268 — "DisableStrike"
static inline int aiBRAIN_DESC_SslFnDisableStrike(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1268); }
// spDesc.pointee[106].__vftable == byte offset 1272 — "LockStrike"
static inline int aiBRAIN_DESC_SslFnLockStrike(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1272); }
// spDesc.pointee[106].ref       == byte offset 1276 — "CancelCurStrike"
static inline int aiBRAIN_DESC_SslFnCancelCurStrike(const dscDESC *d){ return aiBRAIN_DESC_SslFuncId(d, 1276); }
// spDesc.pointee[106].pBrand    == byte offset 1280 — "DbgStrike"
static inline int aiBRAIN_DESC_SslFnDbgStrike(const dscDESC *d)      { return aiBRAIN_DESC_SslFuncId(d, 1280); }
// spDesc.pointee[107].__vftable == byte offset 1284 — "SetWpn"
static inline int aiBRAIN_DESC_SslFnSetWpn(const dscDESC *d)         { return aiBRAIN_DESC_SslFuncId(d, 1284); }
// spDesc.pointee[107].ref       == byte offset 1288 — "SetSpreadAngle"
static inline int aiBRAIN_DESC_SslFnSetSpreadAngle(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1288); }
// spDesc.pointee[107].pBrand    == byte offset 1292 — "SetDamageCurve"
static inline int aiBRAIN_DESC_SslFnSetDamageCurve(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1292); }
// spDesc.pointee[108].ref       == byte offset 1300 — "SetContourShootingDist"
static inline int aiBRAIN_DESC_SslFnSetContourShootingDist(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1300); }
// spDesc.pointee[108].pBrand    == byte offset 1304 — "HasAnyAvailableStrike"
static inline int aiBRAIN_DESC_SslFnHasAnyAvailableStrike(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1304); }

// --- aiBRAIN enemy-lock / movement-dist / behaviour-stack SSL-dispatch batch -----------------------
// (0x8324B388-0x8324BCE8, same descriptor image). Byte offsets verified against each `lwz r5,off(r30)`
// site in the disassembly.
// spDesc.pointee[112].__vftable == byte offset 1344 — "CalcReticleDanger"
static inline int aiBRAIN_DESC_SslFnCalcReticleDanger(const dscDESC *d)      { return aiBRAIN_DESC_SslFuncId(d, 1344); }
// spDesc.pointee[113].ref       == byte offset 1360 — "EnemyResetMovementDist"
static inline int aiBRAIN_DESC_SslFnEnemyResetMovementDist(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1360); }
// spDesc.pointee[113].pBrand    == byte offset 1364 — "EnemyGetMovementDist"
static inline int aiBRAIN_DESC_SslFnEnemyGetMovementDist(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1364); }
// spDesc.pointee[114].__vftable == byte offset 1368 — "ThrowWeaponsAndItems"
static inline int aiBRAIN_DESC_SslFnThrowWeaponsAndItems(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1368); }
// spDesc.pointee[114].ref       == byte offset 1372 — "LockEnemy"
static inline int aiBRAIN_DESC_SslFnLockEnemy(const dscDESC *d)              { return aiBRAIN_DESC_SslFuncId(d, 1372); }
// spDesc.pointee[114].pBrand    == byte offset 1376 — "SpareEnemy"
static inline int aiBRAIN_DESC_SslFnSpareEnemy(const dscDESC *d)             { return aiBRAIN_DESC_SslFuncId(d, 1376); }
// spDesc.pointee[123].ref       == byte offset 1480 — "SetBHV"
static inline int aiBRAIN_DESC_SslFnSetBHV(const dscDESC *d)                 { return aiBRAIN_DESC_SslFuncId(d, 1480); }
// spDesc.pointee[123].pBrand    == byte offset 1484 — "PushBHV"
static inline int aiBRAIN_DESC_SslFnPushBHV(const dscDESC *d)                { return aiBRAIN_DESC_SslFuncId(d, 1484); }
// spDesc.pointee[124].__vftable == byte offset 1488 — "PushSHOOT"
static inline int aiBRAIN_DESC_SslFnPushSHOOT(const dscDESC *d)              { return aiBRAIN_DESC_SslFuncId(d, 1488); }

// --- aiBRAIN event/enemy SSL-dispatch batch (0x832490B0-0x83249A34), same descriptor image -------
// Each reads one per-species SSL event/function id word out of spDesc.pointee and dispatches it via
// sslObject.CallFunc, exactly like the cmd batch above.
// spDesc.pointee[97].__vftable == byte offset 1164 — "OnCreate"
static inline int aiBRAIN_DESC_SslFnOnCreate(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1164); }
// spDesc.pointee[97].ref       == byte offset 1168 — "OnDestroy"
static inline int aiBRAIN_DESC_SslFnOnDestroy(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1168); }
// spDesc.pointee[97].pBrand    == byte offset 1172 — "OnFrameBegin"
static inline int aiBRAIN_DESC_SslFnOnFrameBegin(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1172); }
// spDesc.pointee[98].__vftable == byte offset 1176 — "OnDbgOut"
static inline int aiBRAIN_DESC_SslFnOnDbgOut(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1176); }
// spDesc.pointee[99].__vftable == byte offset 1188 — "DropWeapon"
static inline int aiBRAIN_DESC_SslFnDropWeapon(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1188); }
// spDesc.pointee[99].ref       == byte offset 1192 — "SetEnemy"
static inline int aiBRAIN_DESC_SslFnSetEnemy(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1192); }
// spDesc.pointee[99].pBrand    == byte offset 1196 — "EndEnemy"
static inline int aiBRAIN_DESC_SslFnEndEnemy(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1196); }
// spDesc.pointee[100].__vftable == byte offset 1200 — "EndEnemySoft"
static inline int aiBRAIN_DESC_SslFnEndEnemySoft(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1200); }
// spDesc.pointee[100].ref      == byte offset 1204 — "GetEnemy"
static inline int aiBRAIN_DESC_SslFnGetEnemy(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1204); }
// spDesc.pointee[109].__vftable == byte offset 1308 — "IsLive"
static inline int aiBRAIN_DESC_SslFnIsLive(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1308); }

// --- aiBRAIN behaviour/approach/exclamation SSL-dispatch batch (0x8324BE60-0x8324CCC8), same image -
// spDesc.pointee[103].ref      == byte offset 1240 — "SetApproach"
static inline int aiBRAIN_DESC_SslFnSetApproach(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1240); }
// spDesc.pointee[103].pBrand   == byte offset 1244 — "EndApproach"
static inline int aiBRAIN_DESC_SslFnEndApproach(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1244); }
// spDesc.pointee[111].pBrand   == byte offset 1340 — "SetViewUpCurve"
static inline int aiBRAIN_DESC_SslFnSetViewUpCurve(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1340); }
// spDesc.pointee[115].__vftable == byte offset 1380 — "Exclaim"
static inline int aiBRAIN_DESC_SslFnExclaim(const dscDESC *d)      { return aiBRAIN_DESC_SslFuncId(d, 1380); }
// spDesc.pointee[115].ref      == byte offset 1384 — "OnExStarted"
static inline int aiBRAIN_DESC_SslFnOnExStarted(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1384); }
// spDesc.pointee[115].pBrand   == byte offset 1388 — "OnExPlayed"
static inline int aiBRAIN_DESC_SslFnOnExPlayed(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1388); }
// spDesc.pointee[125].__vftable == byte offset 1500 — "SetBHVOnEmptyStack"
static inline int aiBRAIN_DESC_SslFnSetBHVOnEmptyStack(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1500); }
// spDesc.pointee[125].ref      == byte offset 1504 — "CallBhvFuncOnce"
static inline int aiBRAIN_DESC_SslFnCallBhvFuncOnce(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1504); }
// spDesc.pointee[125].pBrand   == byte offset 1508 — "CancelBHVByNameAndStackAbove"
static inline int aiBRAIN_DESC_SslFnCancelBHVByNameAndStackAbove(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1508); }
// spDesc.pointee[126].__vftable == byte offset 1512 — "FailBHVByNameAndStackAbove"
static inline int aiBRAIN_DESC_SslFnFailBHVByNameAndStackAbove(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1512); }

// --- aiBRAIN nav/exclamation/distance SSL-dispatch batch (0x8324CE08-0x8324E0A8), same descriptor
// image. Each reads one per-species SSL function id word out of spDesc.pointee and dispatches via
// sslObject.CallFunc, exactly like the batches above.
// spDesc.pointee[116].__vftable == byte offset 1392 — "OnExCancelled"
static inline int aiBRAIN_DESC_SslFnOnExCancelled(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1392); }
// spDesc.pointee[116].ref       == byte offset 1396 — "FindExPartner"
static inline int aiBRAIN_DESC_SslFnFindExPartner(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1396); }
// spDesc.pointee[116].pBrand    == byte offset 1400 — "SetNavSys"
static inline int aiBRAIN_DESC_SslFnSetNavSys(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1400); }
// spDesc.pointee[117].ref       == byte offset 1408 — "MoveToPoint"
static inline int aiBRAIN_DESC_SslFnMoveToPoint(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1408); }
// spDesc.pointee[117].pBrand    == byte offset 1412 — "MoveToHO"
static inline int aiBRAIN_DESC_SslFnMoveToHO(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1412); }
// spDesc.pointee[118].__vftable == byte offset 1416 — "MoveToNavSys"
static inline int aiBRAIN_DESC_SslFnMoveToNavSys(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1416); }
// spDesc.pointee[118].ref       == byte offset 1420 — "HasHOInNavSys"
static inline int aiBRAIN_DESC_SslFnHasHOInNavSys(const dscDESC *d){ return aiBRAIN_DESC_SslFuncId(d, 1420); }
// spDesc.pointee[118].pBrand    == byte offset 1424 — "ForgetStuck"
static inline int aiBRAIN_DESC_SslFnForgetStuck(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1424); }
// spDesc.pointee[119].__vftable == byte offset 1428 — "GetDistToObj"
static inline int aiBRAIN_DESC_SslFnGetDistToObj(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1428); }
// spDesc.pointee[119].ref       == byte offset 1432 — "GetDistToWP"
static inline int aiBRAIN_DESC_SslFnGetDistToWP(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1432); }
// spDesc.pointee[119].pBrand    == byte offset 1436 (0x59C) — "GetClosestPoint" (lwz r5,0x59C(r31))
static inline int aiBRAIN_DESC_SslFnGetClosestPoint(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1436); }
// spDesc.pointee[122].pBrand    == byte offset 1472 (0x5C0) — "FindBestTurret" (lwz r5,0x5C0(r31))
static inline int aiBRAIN_DESC_SslFnFindBestTurret(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1472); }

// --- aiBRAIN AF (approach-force?) / QOT ("quote-of-the-front" queue) SSL-dispatch batch
// (0x83251370-0x83251BC0), same descriptor image. Byte offsets verified against each `lwz r5,off(r30)`
// site in the disassembly.
// spDesc.pointee[132].ref       == byte offset 1588 (0x634) — "tUnlock"
static inline int aiBRAIN_DESC_SslFntUnlock(const dscDESC *d)        { return aiBRAIN_DESC_SslFuncId(d, 1588); }
// spDesc.pointee[132].pBrand    == byte offset 1592 (0x638) — "tSetAFCount"
static inline int aiBRAIN_DESC_SslFntSetAFCount(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1592); }
// spDesc.pointee[133].__vftable == byte offset 1596 (0x63C) — "tSetAFDelay"
static inline int aiBRAIN_DESC_SslFntSetAFDelay(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1596); }
// spDesc.pointee[133].ref       == byte offset 1600 (0x640) — "tEnableForceAF"
static inline int aiBRAIN_DESC_SslFntEnableForceAF(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1600); }
// spDesc.pointee[134].ref       == byte offset 1612 (0x64C) — "QOT_Reset"
static inline int aiBRAIN_DESC_SslFnQOT_Reset(const dscDESC *d)      { return aiBRAIN_DESC_SslFuncId(d, 1612); }
// spDesc.pointee[135].ref       == byte offset 1624 (0x658) — "QOT_ClearFront"
static inline int aiBRAIN_DESC_SslFnQOT_ClearFront(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1624); }
// spDesc.pointee[135].pBrand    == byte offset 1628 (0x65C) — "QOT_Size"
static inline int aiBRAIN_DESC_SslFnQOT_Size(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1628); }
// spDesc.pointee[136].__vftable == byte offset 1632 (0x660) — "QOT_IsEmpty"
static inline int aiBRAIN_DESC_SslFnQOT_IsEmpty(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1632); }

// --- aiBRAIN interest/team-name/neutral-count/danger/timer SSL-dispatch batch
// (0x83250648-0x83251350, same descriptor image). Byte offsets = N*12 + field-offset, verified
// against each `lwz r?,off(r30)` site in the disassembly.
// spDesc.pointee[102].ref       == byte offset 1228 — "Alert"
static inline int aiBRAIN_DESC_SslFnAlert(const dscDESC *d)        { return aiBRAIN_DESC_SslFuncId(d, 1228); }
// spDesc.pointee[128].__vftable == byte offset 1536 — "EndInterest"
static inline int aiBRAIN_DESC_SslFnEndInterest(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1536); }
// spDesc.pointee[128].pBrand    == byte offset 1544 — "SetTeamName"
static inline int aiBRAIN_DESC_SslFnSetTeamName(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1544); }
// spDesc.pointee[129].__vftable == byte offset 1548 — "CountMyNeutralsAroundMe"
static inline int aiBRAIN_DESC_SslFnCountMyNeutralsAroundMe(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1548); }
// spDesc.pointee[129].ref       == byte offset 1552 — "CountMyNeutralsAroundEnemy"
static inline int aiBRAIN_DESC_SslFnCountMyNeutralsAroundEnemy(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1552); }
// spDesc.pointee[131].__vftable == byte offset 1572 — "tClear"
static inline int aiBRAIN_DESC_SslFntClear(const dscDESC *d)       { return aiBRAIN_DESC_SslFuncId(d, 1572); }
// spDesc.pointee[131].ref       == byte offset 1576 — "tWaitBHV"
static inline int aiBRAIN_DESC_SslFntWaitBHV(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1576); }
// spDesc.pointee[132].__vftable == byte offset 1584 — "tLock"
static inline int aiBRAIN_DESC_SslFntLock(const dscDESC *d)        { return aiBRAIN_DESC_SslFuncId(d, 1584); }
// spDesc.pointee[136].ref       == byte offset 1636 — "AvoidDanger"
static inline int aiBRAIN_DESC_SslFnAvoidDanger(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1636); }
// spDesc.pointee[136].pBrand    == byte offset 1640 — "AvoidGrenade"
static inline int aiBRAIN_DESC_SslFnAvoidGrenade(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1640); }

// --- aiBRAIN param/timer/group SSL-dispatch batch (0x8324F798-0x832505E8), same descriptor image.
// Byte offsets verified against each `lwz r5,off(rN)` site in the disassembly.
// spDesc.pointee[110].ref       == byte offset 1324 — "GetBool"
static inline int aiBRAIN_DESC_SslFnGetBool(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1324); }
// spDesc.pointee[110].pBrand    == byte offset 1328 — "GetInt"
static inline int aiBRAIN_DESC_SslFnGetInt(const dscDESC *d)      { return aiBRAIN_DESC_SslFuncId(d, 1328); }
// spDesc.pointee[112].ref       == byte offset 1348 — "SetTimer"
static inline int aiBRAIN_DESC_SslFnSetTimer(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1348); }
// spDesc.pointee[112].pBrand    == byte offset 1352 — "KillTimer"
static inline int aiBRAIN_DESC_SslFnKillTimer(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1352); }
// spDesc.pointee[113].__vftable == byte offset 1356 — "OnTimer"
static inline int aiBRAIN_DESC_SslFnOnTimer(const dscDESC *d)     { return aiBRAIN_DESC_SslFuncId(d, 1356); }
// spDesc.pointee[126].ref       == byte offset 1516 — "JoinGroup"
static inline int aiBRAIN_DESC_SslFnJoinGroup(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1516); }
// spDesc.pointee[126].pBrand    == byte offset 1520 — "LeaveGroup"
static inline int aiBRAIN_DESC_SslFnLeaveGroup(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1520); }
// spDesc.pointee[127].__vftable == byte offset 1524 — "HaveGroup"
static inline int aiBRAIN_DESC_SslFnHaveGroup(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1524); }
// spDesc.pointee[127].ref       == byte offset 1528 — "GetGroup"
static inline int aiBRAIN_DESC_SslFnGetGroup(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1528); }
// spDesc.pointee[127].pBrand    == byte offset 1532 — "SetInterest"
static inline int aiBRAIN_DESC_SslFnSetInterest(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1532); }

// --- aiBRAIN float-param + smart-object/turret/force-speed SSL-dispatch batch
// (0x8324E4D8-0x8324F5F8), same descriptor image. Each reads one per-species SSL function id word
// out of spDesc.pointee and dispatches via sslObject.CallFunc, exactly like the batches above.
// spDesc.pointee[109].pBrand    == byte offset 1316 — "GetFloat"
static inline int aiBRAIN_DESC_SslFnGetFloat(const dscDESC *d)              { return aiBRAIN_DESC_SslFuncId(d, 1316); }
// spDesc.pointee[110].__vftable == byte offset 1320 — "SetFloat"
static inline int aiBRAIN_DESC_SslFnSetFloat(const dscDESC *d)              { return aiBRAIN_DESC_SslFuncId(d, 1320); }
// spDesc.pointee[120].__vftable == byte offset 1440 — "GetPathRemain"
static inline int aiBRAIN_DESC_SslFnGetPathRemain(const dscDESC *d)         { return aiBRAIN_DESC_SslFuncId(d, 1440); }
// spDesc.pointee[120].ref       == byte offset 1444 — "GetDistPlayerToPathEnd"
static inline int aiBRAIN_DESC_SslFnGetDistPlayerToPathEnd(const dscDESC *d){ return aiBRAIN_DESC_SslFuncId(d, 1444); }
// spDesc.pointee[120].pBrand    == byte offset 1448 — "SetForceSpeed"
static inline int aiBRAIN_DESC_SslFnSetForceSpeed(const dscDESC *d)         { return aiBRAIN_DESC_SslFuncId(d, 1448); }
// spDesc.pointee[121].__vftable == byte offset 1452 — "EndForceSpeed"
static inline int aiBRAIN_DESC_SslFnEndForceSpeed(const dscDESC *d)         { return aiBRAIN_DESC_SslFuncId(d, 1452); }
// spDesc.pointee[121].ref       == byte offset 1456 — "UseSmartObject"
static inline int aiBRAIN_DESC_SslFnUseSmartObject(const dscDESC *d)        { return aiBRAIN_DESC_SslFuncId(d, 1456); }
// spDesc.pointee[121].pBrand    == byte offset 1460 — "UseSmartObjectInstant"
static inline int aiBRAIN_DESC_SslFnUseSmartObjectInstant(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1460); }
// spDesc.pointee[122].__vftable == byte offset 1464 — "AttachBodyToSmtObj"
static inline int aiBRAIN_DESC_SslFnAttachBodyToSmtObj(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1464); }
// spDesc.pointee[122].ref       == byte offset 1468 — "UseTurret"
static inline int aiBRAIN_DESC_SslFnUseTurret(const dscDESC *d)             { return aiBRAIN_DESC_SslFuncId(d, 1468); }
// spDesc.pointee[123].__vftable == byte offset 1476 — "UseSmartIdleInstant"
static inline int aiBRAIN_DESC_SslFnUseSmartIdleInstant(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1476); }

// --- aiBRAIN hideout-event / GetAim SSL-dispatch batch (0x83255950-0x83256848), same descriptor
// image. Byte offsets verified against each `lwz r5,off(r30/r29)` site in the disassembly.
// spDesc.pointee[95].__vftable == byte offset 1140 (0x474) — "OnEnterHO" (sslObject.HandleEvent id)
static inline int aiBRAIN_DESC_SslFnOnEnterHO(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1140); }
// spDesc.pointee[95].ref       == byte offset 1144 (0x478) — "OnLeaveHO" (sslObject.HandleEvent id)
static inline int aiBRAIN_DESC_SslFnOnLeaveHO(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1144); }
// spDesc.pointee[94].__vftable == byte offset 1128 (0x468) — "OnApproach" enter event id
static inline int aiBRAIN_DESC_SslEvOnApproachEnter(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1128); }
// spDesc.pointee[94].ref       == byte offset 1132 (0x46C) — "OnApproach" leave event id
static inline int aiBRAIN_DESC_SslEvOnApproachLeave(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1132); }
// spDesc.pointee[101].pBrand   == byte offset 1220 (0x4C4) — "GetAim"
static inline int aiBRAIN_DESC_SslFnGetAim(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1220); }
// byte offset 0x510 == 1296 (lwz r5,0x510(spDesc.pointee)) — "AIGetCurWeaponClassName"
static inline int aiBRAIN_DESC_SslFnAIGetCurWeaponClassName(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1296); }
// byte offset 0x5D4 == 1492 (lwz r5,0x5D4(spDesc.pointee)) — "GetBHV"
static inline int aiBRAIN_DESC_SslFnGetBHV(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1492); }

// --- aiBRAIN get/set-param + team/interest/join-group SSL-dispatch batch (0x83258B58-0x832594C0),
// same descriptor image. Byte offsets verified against each `lwz r5,off(rN)` site in the disassembly.
// spDesc.pointee[111].__vftable == byte offset 1332 (0x534) — "GetString"
static inline int aiBRAIN_DESC_SslFnGetString(const dscDESC *d)  { return aiBRAIN_DESC_SslFuncId(d, 1332); }
// spDesc.pointee[128].pBrand    == byte offset 1540 (0x604) — "GetTeamName"
static inline int aiBRAIN_DESC_SslFnGetTeamName(const dscDESC *d){ return aiBRAIN_DESC_SslFuncId(d, 1540); }
// spDesc.pointee[133].pBrand    == byte offset 1604 (0x644) — "TestParam2AI"
static inline int aiBRAIN_DESC_SslFnTestParam2AI(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1604); }
// spDesc.pointee[134].pBrand    == byte offset 1616 (0x650) — "QOT_Put"
static inline int aiBRAIN_DESC_SslFnQOT_Put(const dscDESC *d)    { return aiBRAIN_DESC_SslFuncId(d, 1616); }

// --- aiBRAIN Get*-string SSL accessor batch (0x83257xxx, same descriptor image). Byte offsets
// verified against each `lwz r5,off(r29)` site in the disassembly.
// spDesc.pointee[124].pBrand   == byte offset 1496 (0x5D8) — "GetMajorBHVType"
static inline int aiBRAIN_DESC_SslFnGetMajorBHVType(const dscDESC *d)   { return aiBRAIN_DESC_SslFuncId(d, 1496); }
// spDesc.pointee[111].__vftable == byte offset 1336 (0x538) — "GetNameSpawnedFrom"
static inline int aiBRAIN_DESC_SslFnGetNameSpawnedFrom(const dscDESC *d) { return aiBRAIN_DESC_SslFuncId(d, 1336); }
// spDesc.pointee[117].__vftable == byte offset 1404 (0x57C) — "GetNavSys"
static inline int aiBRAIN_DESC_SslFnGetNavSys(const dscDESC *d)         { return aiBRAIN_DESC_SslFuncId(d, 1404); }
