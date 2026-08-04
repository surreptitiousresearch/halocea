// aiBASS::ProcessStream @ 0x832453C8
// ?ProcessStream@aiBASS@@MAAXM@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// Per-frame stream tick. For each in-flight exclamation:
//   * not yet started  -> try to Start(); on failure notify the owner (OnExCancelled) and reap it.
//   * already started  -> if it just finished, notify the owner (OnExPlayed); reap it once expired,
//                         otherwise advance its elapsed timer by `dt`.
// `totalTime > 0` distinguishes a started entry from a pending one (it is stamped on Start()).
void aiBASS::ProcessStream(float dt)
{
    ExStreamIter it(exStream.Begin());
    while (!it.IsDone())
    {
        exSTREAM_DATA *entry = it.value();

        if (entry->totalTime > 0.000001f)
        {
            if (entry->IsJustFinished())
            {
                ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = entry->owner.pHandle;
                aiBRAIN *brain = handle ? handle->pPtr : nullptr;
                if (brain)
                {
                    dsTSTRING<char> exName(entry->name.CStr());
                    brain->OnExPlayed(exName);
                }
            }

            if (entry->IsExpired())
            {
                it = exStream.Erase(it);
                continue;
            }

            if (entry->totalTime > 0.000001f)
                entry->time += dt;
            ++it;
        }
        else
        {
            if (!entry->Start())
            {
                ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = entry->owner.pHandle;
                aiBRAIN *brain = handle ? handle->pPtr : nullptr;
                if (brain)
                {
                    dsTSTRING<char> exName(entry->name.CStr());
                    brain->OnExCancelled(exName);
                }
                it = exStream.Erase(it);
                continue;
            }
            ++it;
        }
    }
}
