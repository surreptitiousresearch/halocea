#include "../headers/havok/hkFileSystem_boundary.h"
#include "../headers/havok/win32_file_boundary.h"
#include "../headers/havok/hkThreadMemory.h"

/* Enumerate a directory into an hkFileSystem::DirectoryListing. The input path is
   copied into a scratch buffer, given a trailing slash, converted to a platform
   path, then a "*" wildcard is appended for FindFirstFileA. Entries other than
   "." / ".." are added as directories or files (with a converted write time).
   Returns 1 if the target is missing or not a directory, else 0.
   ("local variable allocation has failed" in the decompiler — several reg-alloc
   fusions noted inline.) */
int hkWin32ListDirectory(char *pathIn, hkFileSystem_DirectoryListing *out)
{
    hkArray<char> platformPath;
    char *target;
    unsigned int attrs;
    int len = hkString_strLen(pathIn);

    platformPath.m_data = 0;
    platformPath.m_size = 0;
    platformPath.m_capacityAndFlags = 0x80000000;

    if (len > 0)
    {
        hkArray<char> work;
        char last;

        work.m_data = 0;
        work.m_size = 0;
        work.m_capacityAndFlags = 0x80000000;
        hkArrayUtil__reserve(&work, len + 3, 1);
        work.m_size = len + 3;
        hkString_memCpy(work.m_data, pathIn, len);
        ((char *)work.m_data)[len] = 0;

        last = ((char *)work.m_data)[len - 1];
        if (last != '/' && last != '\\')
        {
            ((char *)work.m_data)[len] = '/';
            ((char *)work.m_data)[len + 1] = 0;
        }

        hkNativeFileSystem_s_havokToPlatformConvert((char *)work.m_data, &platformPath);
        if ((int)work.m_capacityAndFlags >= 0) /* heap-owned (not the dont-deallocate flag) */
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), work.m_data,
                                           work.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
    }

    target = platformPath.m_size ? (char *)platformPath.m_data : pathIn;
    attrs = GetFileAttributesA(target);

    if (attrs == 0xFFFFFFFF || ((attrs >> 4) & 1) == 0) /* missing or not a directory */
    {
        if ((int)platformPath.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), platformPath.m_data,
                                           platformPath.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
        return 1;
    }
    else
    {
        _WIN32_FIND_DATAA findData;
        void *handle;

        if (platformPath.m_size)
        {
            /* overwrite the trailing slash with the "*" wildcard, keep null-terminated */
            ((char *)platformPath.m_data)[platformPath.m_size - 1] = '*';
            if (platformPath.m_size == (platformPath.m_capacityAndFlags & 0x3FFFFFFF))
                hkArrayUtil__reserveMore(&platformPath, 1);
            ((char *)platformPath.m_data)[platformPath.m_size] = 0;
            target = (char *)platformPath.m_data;
            ++platformPath.m_size;
        }

        handle = FindFirstFileA(target, &findData);
        if (handle != (void *)-1)
        {
            do
            {
                if (hkString_strCmp(findData.cFileName, ".") && hkString_strCmp(findData.cFileName, ".."))
                {
                    if ((findData.dwFileAttributes >> 4) & 1)
                    {
                        hkFileSystem_DirectoryListing_addDirectory(out, findData.cFileName);
                    }
                    else
                    {
                        /* DEVIATION: reg-alloc fused the 64-bit divisor 10000000 with the ".."
                           pointer; the original converts the write time (100ns FILETIME units +
                           epoch offset) into seconds. */
                        unsigned long long writeTime =
                            (unsigned long long)(findData.ftLastWriteTime.dwLowDateTime + 717324288u) / 10000000u;
                        hkFileSystem_DirectoryListing_addFile(out, findData.cFileName, writeTime);
                    }
                }
            } while (FindNextFileA(handle, &findData));
        }
        CloseHandle(handle);

        if ((int)platformPath.m_capacityAndFlags >= 0)
            hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(), platformPath.m_data,
                                           platformPath.m_capacityAndFlags & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
        return 0;
    }
}
