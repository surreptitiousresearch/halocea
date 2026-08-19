/* scenario_name_to_cache_file_path @0x83753FF0 */
extern char *cache_files_map_directory(void);
extern const char *cache_files_root_directory(void);
extern int sprintf_0(char *string, const char *format, ...);

void scenario_name_to_cache_file_path(const char *scenario_name, char *path)
{
    const char *map_dir = cache_files_map_directory();
    const char *root_dir = cache_files_root_directory();

    sprintf_0(path, "%s%s%s.map", root_dir, map_dir, scenario_name);
}
