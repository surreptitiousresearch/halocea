/* ?InitHW@d3dDRIVER@@MAAHXZ @0x82690EF0 */
#include "headers/d3d_driver.h"

// --- boundary externs (deeper than this wave's descent) ---
struct vidLOCK { void Lock(const char *file, int line); void Unlock(const char *file, int line); };
extern vidLOCK vidLock;
extern void _apLog(const char *fmt, ...);

// 0x82690EF0 -- d3dDRIVER::InitHW. When the device-ready bit (0x04000000) is clear, create the
// Xenon device (InitHWxenon); on failure log FATAL, drop the video lock, tear down and fail.
// On success chain vidDRIVER::InitHW, seed the shared render state and occlusion queries, then
// mark the driver ready. Returns 1 (already-ready path is also success).
int d3dDRIVER::InitHW()
{
    if ( ((this->base.state.state >> 26) & 1) == 0 )
    {
        if ( !this->InitHWxenon() )
        {
            _apLog("~D3d,Error~FATAL: Initialization failed\n");
            vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 889);
            this->TermHW();
            return 0;
        }
        _apLog("~D3d,Info~Initialization complete successifully\n");
        if ( !this->base.InitHW() ) // base-subobject call (composition model) (2026-07-31)
        {
            vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 878);
            return 0;
        }
        this->SetStartRenderingState();
        this->RecreateQueries();
        this->base.state.state |= 0x4000000u;
        vidLock.Unlock("D:\\Projects\\code\\common\\src.sys\\drv\\video\\d3d_8\\D3d_drv_8.cpp", 885);
    }
    return 1;
}
