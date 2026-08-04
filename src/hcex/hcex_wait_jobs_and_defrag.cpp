/* hcex_wait_jobs_and_defrag @0x823D13E8 — when called from inside the ws-engine ProcessFrame job, drain the
 * job manager (hcex_wait_jobs) and then take+release the video lock, giving the driver a defrag/settle point
 * while all worker threads are idle. Outside ProcessFrame it is a no-op. vidLOCK is a ws-engine boundary. */

extern unsigned char hcex_inside_processframe;
extern void hcex_wait_jobs(void);

struct vidLOCK;
extern struct vidLOCK vidLock;
extern void vidLOCK_Lock(struct vidLOCK *lock, const char *file, int line);
extern void vidLOCK_Unlock(struct vidLOCK *lock, const char *file, int line);

extern "C" void hcex_wait_jobs_and_defrag(void)
{
    if ( hcex_inside_processframe )
    {
        hcex_wait_jobs();
        vidLOCK_Lock(&vidLock, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 3197);
        vidLOCK_Unlock(&vidLock, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 3197);
    }
}
