/* cache_files_enable_writes @0x836F7028 */
extern unsigned int XPhysicalProtect(void *BaseAddress, unsigned int RegionSize, unsigned int Protect);

void cache_files_enable_writes(void)
{
    XPhysicalProtect((void *)0x4B8000, 0x1000000u, 4u);
}
