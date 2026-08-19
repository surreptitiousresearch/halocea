/* cache_files_disable_writes @0x836F7040 */
extern unsigned int XPhysicalProtect(void *BaseAddress, unsigned int RegionSize, unsigned int Protect);

void cache_files_disable_writes(void)
{
    XPhysicalProtect((void *)0x4B8000, 0x1000000u, 2u);
}
