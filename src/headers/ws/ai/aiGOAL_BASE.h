#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../m3d/m3dSPL_LINEAR1D.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsPAIR.h"
#include "aiSPD.h"
// ws-engine ai08: base class for a brain's goal/navigation-behaviour module.
// DB-verified layout (types_members aiGOAL_BASE): __vftable@0, invalidSpeed@4 (aiSPD, 52) — size 56.

struct aiGOAL_BASE_vtbl;
struct navPATH; // nav subsystem — boundary (fwd)
struct navWP;   // nav waypoint  — boundary (fwd)
struct navSYS;  // nav subsystem — boundary (fwd)
struct m3dMATR; // 4x3 affine matrix — boundary (fwd)
struct psSECTION;
struct smtSMART_OBJ_BASE; // smt subsystem — boundary (fwd)
struct aiHO_SYS;          // ai hideout system — boundary (fwd, for member-fn-ptr param)
struct navWFUNC;          // nav weighting functor — boundary (fwd)
struct navWFUNC_DIST;     // nav distance weighting functor — boundary (fwd)
struct navWP_ID_WEIGHT;   // nav (wpid, weight) pair
struct navASSOC_INFO;     // nav waypoint-association debug record
struct animINST;          // anim instance — boundary (fwd)
struct m3dV;              // 3-vector
struct m3dSPL_LINEAR1D;
template<class T, int N> struct dsVECTOR;

struct aiGOAL_BASE {
    aiGOAL_BASE_vtbl *__vftable; // 0x00
    aiSPD             invalidSpeed; // 0x04

    // 0x831792D8 (?BuildCompoundPath@aiGOAL_BASE@@UAA_NABV?$dsTSTRING@D@@PAVnavPATH@@_NAAV2@@Z)
    // — virtual. Base default: report the feature disabled — write "disabled" into `errHint`
    // and return false (no path built).
    virtual bool BuildCompoundPath(const dsTSTRING<char> &dest, navPATH *pPath,
                                   bool isNeedHO, dsTSTRING<char> &errHint);

    // 0x83179380 (?GetCmpPathDestWP@aiGOAL_BASE@@UAA...) — virtual. Base default: report the
    // feature disabled — write "disabled" into `errHint` and return null (no destination waypoint).
    virtual navWP *GetCmpPathDestWP(const dsTSTRING<char> *dest, short *destWPID,
                                    navSYS **destNS, bool isNeedHO, dsTSTRING<char> *errHint);

    // 0x83179428 (?BuildPathToNS@aiGOAL_BASE@@UAA...) — virtual. Base default: report the feature
    // disabled — write "disabled" into `errHint` and return 0 (no path built).
    virtual int BuildPathToNS(short wpidFrom, const m3dMATR *mFrom, const navSYS *srcNS,
                              const navSYS *dstNS, navPATH *pPath, dsTSTRING<char> *errHint);

    // ?BuildPathToSmtObj@aiGOAL_BASE@@UAA_NAAV?$dsSHARED_PTR@VsmtSMART_OBJ_BASE@@$0A@V?$Deleter@Vsmt
    // SMART_OBJ_BASE@@@@@@PAVnavPATH@@AAV?$dsPAIR@VdsSTRID@@V1@@@_N@Z — virtual: build a nav path onto
    // the smart object, returning the chosen entry-point (source/dest STRID pair) via `entryPointInfo`.
    virtual bool BuildPathToSmtObj(
        dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
        navPATH *pPath, dsPAIR<dsSTRID, dsSTRID> &entryPointInfo, bool isNeedHO);

    // Parse a "x0;y0|x1;y1|..." control-point list into a linear spline.  static, boundary.
    static void ParseSpline(const dsTSTRING<char> *text, m3dSPL_LINEAR1D *out);

    // ?SetForceSpeed@aiGOAL_BASE@@UAA_NXZ — virtual: try to apply the pending forced gait; returns
    // false when no forced gait could be set.  boundary — dispatched by aiBRAIN::implSetForceSpeed.
    bool SetForceSpeed();

    // ?UpdateTimers@aiGOAL_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // vtbl — force-clear the goal module (teardown). Used by aiMIND::ForcedClear.  boundary.
    void ForcedClear();

    // vtbl slot 4 (byte offset 0x10) — the nav system this goal module is currently localized to
    // (null when none). Dispatched by DB-verified slot index (as aiBODY_IFACE::GetBodyProp does)
    // rather than fabricating the preceding virtual declarations. Used by aiBRAIN::implGetNavSys.
    navSYS *GetNavSystem() {
        typedef navSYS *(*Fn)(aiGOAL_BASE *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[4];
        return fn(this);
    }

    // vtbl — localize this goal module to nav system `ns` (isForce re-seats it even when already set).
    // Used by aiBRAIN::ForceAFMoveToNavSys / ForceAFMoveToHO.  boundary — body external to this batch.
    void SetNavSystem(navSYS *ns, bool isForce);

    // vtbl (byte offset 0x50) — decide whether the HO at `howp` is acceptable, applying the
    // reconsider `checker` (an aiHO_SYS member-fn pointer) under enable-mask `mode`, optionally
    // requiring the check against all enemies. Called by aiHO_SYS::AdvCheckAcceptHO.  boundary.
    bool IsAcceptHO(const navWP *howp,
                    bool (aiHO_SYS::*checker)(const m3dV &) const,
                    unsigned int mode, bool *checkAllEnemies);

    // 0x8329B298 (?GetHO@aiGOAL_BASE@@SAPAVnavWP@@F@Z) — static: the high-obstacle waypoint with id
    // `wpid` from the global planner, or null when the id is invalid / absent / not an HO. REVERSED.
    static navWP *GetHO(short wpid);

    // 0x8329B380 (?IsEqualSplines@aiGOAL_BASE@@SA_NABVm3dSPL_LINEAR1D@@0M@Z) — static: true when the
    // two 1-D splines agree (to `precision`) at every keyframe argument of either. REVERSED.
    static bool IsEqualSplines(const m3dSPL_LINEAR1D &spline1,
                               const m3dSPL_LINEAR1D &spline2, float precision);

    // 0x8329BD28 (?FilterWPsByWeight@aiGOAL_BASE@@SAX...) — static: reweight each waypoint of `src`
    // by `wfun` (adding the existing weight), keep those under `maxWeight`, optionally sort, and copy
    // up to `maxCount` (all when <0) into `dst`. REVERSED.
    static void FilterWPsByWeight(const navWFUNC *wfun, float maxWeight,
                                  const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                  dsVECTOR<navWP_ID_WEIGHT, 8> &dst,
                                  bool doSort, int maxCount);

    // 0x8329BF80 (?FilterWPsByPenalty@aiGOAL_BASE@@SAX...) — static: add each waypoint's dynamic
    // navigation penalty to its weight, keeping those under `maxWeight` in `dst`. REVERSED.
    static void FilterWPsByPenalty(float maxWeight,
                                   const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                   dsVECTOR<navWP_ID_WEIGHT, 8> &dst);

    // 0x8329C190 (?FilterWPsByWeightAssc@aiGOAL_BASE@@SAX...) — static: reweight+sort like
    // FilterWPsByWeight, then keep up to `maxCount` waypoints that pass an HO-acquirability gate and
    // a thin-visibility check from `wfun`'s center (offset by `shift`), recording per-candidate
    // association info into `dbgAssoc` when supplied. REVERSED.
    static void FilterWPsByWeightAssc(animINST *pInst, const navWFUNC_DIST *wfun, float maxWeight,
                                      int maxCount, bool gateHO, const m3dV &shift,
                                      unsigned int mask,
                                      const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                      dsVECTOR<navWP_ID_WEIGHT, 8> &dst,
                                      dsVECTOR<navASSOC_INFO, 8> *dbgAssoc);
};
