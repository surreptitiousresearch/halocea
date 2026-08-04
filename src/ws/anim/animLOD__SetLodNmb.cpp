#include "animLOD_LOD_INFO.h"
#include "../ds/ds_assert_boundary.h"
#include <math.h>

// ?SetLodNmb@animLOD@@AAA_NHM@Z  (0x82B0DFA0, private) — switch the active LOD level to `lodNmbNew`
// with cross-fade factor `lodFactorNew`, hiding the previously-shown level(s) and showing the new
// one(s). Returns true when the level or factor actually changed.
// The `lodFactorNew` parameter is a float (mangle M); the decompiler widened it to double.
bool animLOD::SetLodNmb(int lodNmbNew, float lodFactorNew)
{
    if (!this->lods.nElem)
        return false;

    int lodNmbCur = this->lodNmbCur;
    if (lodNmbNew == lodNmbCur && fabsf(this->lodFactorCur - lodFactorNew) < 1e-6f)
        return false;

    if (lodNmbCur != -1)
    {
        this->lods[lodNmbCur].SetVisibility(false);
        if (fabsf(this->lodFactorCur) >= 1e-6f)
        {
            if (!IGNORE_STRONG_ASSERT && this->lodNmbCur <= 0)
                STRONG_ASSERT2_HELPER::asserd<int>(
                    "lodNmbCur > 0",
                    "D:\\Projects\\code\\common\\src.sys\\objects\\lod.cpp",
                    254,
                    "lodNmbCur", this->lodNmbCur);
            this->lods[this->lodNmbCur - 1].SetVisibility(false);
        }
    }

    this->lodFactorCur = lodFactorNew;
    this->lodNmbCur = lodNmbNew;

    if (lodNmbNew != -1)
    {
        if (fabsf(lodFactorNew) >= 1e-6f)
        {
            if (!IGNORE_STRONG_ASSERT && lodNmbNew <= 0)
                STRONG_ASSERT2_HELPER::asserd<int>(
                    "lodNmbCur > 0",
                    "D:\\Projects\\code\\common\\src.sys\\objects\\lod.cpp",
                    271,
                    "lodNmbCur", this->lodNmbCur);
            this->lods[this->lodNmbCur - 1].SetVisibility(true);
        }
        this->lods[this->lodNmbCur].SetVisibility(true);
    }

    return true;
}
