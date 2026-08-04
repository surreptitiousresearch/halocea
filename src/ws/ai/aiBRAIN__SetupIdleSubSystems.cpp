#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/ai_setup_subsystems_boundary.h" // aiAIM_IDLE / aiVISION_IDLE / aiISA
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiBRAIN::SetupIdleSubSystems() @ 0x832A1880
// ?SetupIdleSubSystems@aiBRAIN@@MAAXXZ (protected virtual)
// Swap this brain's aim / vision / interop(isa) mind modules for their lightweight "idle" variants,
// releasing whatever module was installed before. (a8_isa.cpp)
namespace {
const char kSrcFile[] = "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_isa.cpp";
}

void aiBRAIN::SetupIdleSubSystems()
{
    // --- aim -> aiAIM_IDLE ---
    {
        aiAIM_IDLE *idle = new (kSrcFile, 0x9C) aiAIM_IDLE(this);
        dsSHARED_PTR<aiAIM_BASE, 0, Deleter<aiAIM_BASE> > holder((aiAIM_BASE *)idle);
        aiAIM_BASE *oldPointee = holder.pointee;
        holder.pointee = this->mind.aim.pointee;
        this->mind.aim.pointee = oldPointee;
        int oldCounter = holder.counter;
        holder.counter = this->mind.aim.counter;
        this->mind.aim.counter = oldCounter;
        holder.Reset();
    }

    // --- vision -> aiVISION_IDLE ---
    {
        aiVISION_IDLE *idle = new (kSrcFile, 0x9D) aiVISION_IDLE(this);
        dsSHARED_PTR<aiVISION_BASE, 0, Deleter<aiVISION_BASE> > holder((aiVISION_BASE *)idle);
        aiVISION_BASE *oldPointee = holder.pointee;
        holder.pointee = this->mind.vision.pointee;
        this->mind.vision.pointee = oldPointee;
        int oldCounter = holder.counter;
        holder.counter = this->mind.vision.counter;
        this->mind.vision.counter = oldCounter;
        holder.Reset();
    }

    // --- isa -> aiISA (concrete interop module) ---
    {
        aiISA *module = new (kSrcFile, 0x9E) aiISA(this);
        dsSHARED_PTR<aiISA_BASE, 0, Deleter<aiISA_BASE> > holder((aiISA_BASE *)module);
        aiISA_BASE *oldPointee = holder.pointee;
        holder.pointee = this->mind.isa.pointee;
        this->mind.isa.pointee = oldPointee;
        int oldCounter = holder.counter;
        holder.counter = this->mind.isa.counter;
        this->mind.isa.counter = oldCounter;
        holder.Reset();
    }
}
