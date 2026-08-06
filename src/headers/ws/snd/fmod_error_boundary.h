#pragma once
/* External/SDK extern boundary — the FMOD Ex SDK (Firelight Technologies) is NOT reversed; only the
 * pieces the two FMOD helper free functions touch are declared here.
 *
 * Boundary declarations for:
 *   - FModErrorDesc  (src/ws/snd/FModErrorDesc.cpp)  — FMOD_RESULT -> human/enum string
 *   - WaitSoundBank  (src/hcex/WaitSoundBank.cpp)    — spin until a streaming sound finishes loading
 *
 * FMOD_RESULT is the full DB enumerator set (types_enum_values FMOD_RESULT, 96 members). FMOD::Sound
 * is the SDK class (opaque; only getOpenState is called). */

/* ---- FMOD_RESULT (full DB enumerator set) ---- */
enum FMOD_RESULT {
    FMOD_OK = 0,
    FMOD_ERR_ALREADYLOCKED = 1,
    FMOD_ERR_BADCOMMAND = 2,
    FMOD_ERR_CDDA_DRIVERS = 3,
    FMOD_ERR_CDDA_INIT = 4,
    FMOD_ERR_CDDA_INVALID_DEVICE = 5,
    FMOD_ERR_CDDA_NOAUDIO = 6,
    FMOD_ERR_CDDA_NODEVICES = 7,
    FMOD_ERR_CDDA_NODISC = 8,
    FMOD_ERR_CDDA_READ = 9,
    FMOD_ERR_CHANNEL_ALLOC = 10,
    FMOD_ERR_CHANNEL_STOLEN = 11,
    FMOD_ERR_COM = 12,
    FMOD_ERR_DMA = 13,
    FMOD_ERR_DSP_CONNECTION = 14,
    FMOD_ERR_DSP_FORMAT = 15,
    FMOD_ERR_DSP_NOTFOUND = 16,
    FMOD_ERR_DSP_RUNNING = 17,
    FMOD_ERR_DSP_TOOMANYCONNECTIONS = 18,
    FMOD_ERR_FILE_BAD = 19,
    FMOD_ERR_FILE_COULDNOTSEEK = 20,
    FMOD_ERR_FILE_DISKEJECTED = 21,
    FMOD_ERR_FILE_EOF = 22,
    FMOD_ERR_FILE_NOTFOUND = 23,
    FMOD_ERR_FILE_UNWANTED = 24,
    FMOD_ERR_FORMAT = 25,
    FMOD_ERR_HTTP = 26,
    FMOD_ERR_HTTP_ACCESS = 27,
    FMOD_ERR_HTTP_PROXY_AUTH = 28,
    FMOD_ERR_HTTP_SERVER_ERROR = 29,
    FMOD_ERR_HTTP_TIMEOUT = 30,
    FMOD_ERR_INITIALIZATION = 31,
    FMOD_ERR_INITIALIZED = 32,
    FMOD_ERR_INTERNAL = 33,
    FMOD_ERR_INVALID_ADDRESS = 34,
    FMOD_ERR_INVALID_FLOAT = 35,
    FMOD_ERR_INVALID_HANDLE = 36,
    FMOD_ERR_INVALID_PARAM = 37,
    FMOD_ERR_INVALID_POSITION = 38,
    FMOD_ERR_INVALID_SPEAKER = 39,
    FMOD_ERR_INVALID_SYNCPOINT = 40,
    FMOD_ERR_INVALID_VECTOR = 41,
    FMOD_ERR_IRX = 42,
    FMOD_ERR_MAXAUDIBLE = 43,
    FMOD_ERR_MEMORY = 44,
    FMOD_ERR_MEMORY_CANTPOINT = 45,
    FMOD_ERR_MEMORY_IOP = 46,
    FMOD_ERR_MEMORY_SRAM = 47,
    FMOD_ERR_NEEDS2D = 48,
    FMOD_ERR_NEEDS3D = 49,
    FMOD_ERR_NEEDSHARDWARE = 50,
    FMOD_ERR_NEEDSSOFTWARE = 51,
    FMOD_ERR_NET_CONNECT = 52,
    FMOD_ERR_NET_SOCKET_ERROR = 53,
    FMOD_ERR_NET_URL = 54,
    FMOD_ERR_NET_WOULD_BLOCK = 55,
    FMOD_ERR_NOTREADY = 56,
    FMOD_ERR_OUTPUT_ALLOCATED = 57,
    FMOD_ERR_OUTPUT_CREATEBUFFER = 58,
    FMOD_ERR_OUTPUT_DRIVERCALL = 59,
    FMOD_ERR_OUTPUT_ENUMERATION = 60,
    FMOD_ERR_OUTPUT_FORMAT = 61,
    FMOD_ERR_OUTPUT_INIT = 62,
    FMOD_ERR_OUTPUT_NOHARDWARE = 63,
    FMOD_ERR_OUTPUT_NOSOFTWARE = 64,
    FMOD_ERR_PAN = 65,
    FMOD_ERR_PLUGIN = 66,
    FMOD_ERR_PLUGIN_INSTANCES = 67,
    FMOD_ERR_PLUGIN_MISSING = 68,
    FMOD_ERR_PLUGIN_RESOURCE = 69,
    FMOD_ERR_RECORD = 70,
    FMOD_ERR_REVERB_INSTANCE = 71,
    FMOD_ERR_SUBSOUND_ALLOCATED = 72,
    FMOD_ERR_SUBSOUND_CANTMOVE = 73,
    FMOD_ERR_SUBSOUND_MODE = 74,
    FMOD_ERR_SUBSOUNDS = 75,
    FMOD_ERR_TAGNOTFOUND = 76,
    FMOD_ERR_TOOMANYCHANNELS = 77,
    FMOD_ERR_UNIMPLEMENTED = 78,
    FMOD_ERR_UNINITIALIZED = 79,
    FMOD_ERR_UNSUPPORTED = 80,
    FMOD_ERR_UPDATE = 81,
    FMOD_ERR_VERSION = 82,
    FMOD_ERR_PRELOADED = 83,
    FMOD_ERR_EVENT_FAILED = 84,
    FMOD_ERR_EVENT_INFOONLY = 85,
    FMOD_ERR_EVENT_INTERNAL = 86,
    FMOD_ERR_EVENT_MAXSTREAMS = 87,
    FMOD_ERR_EVENT_MISMATCH = 88,
    FMOD_ERR_EVENT_NAMECONFLICT = 89,
    FMOD_ERR_EVENT_NOTFOUND = 90,
    FMOD_ERR_EVENT_NEEDSSIMPLE = 91,
    FMOD_ERR_EVENT_GUIDCONFLICT = 92,
    FMOD_ERR_EVENT_ALREADY_LOADED = 93,
    FMOD_ERR_MUSIC_UNINITIALIZED = 94,
    FMOD_RESULT_FORCEINT = 65536,
};

/* ---- FMOD_OPENSTATE (types_enum_values FMOD_OPENSTATE) ---- */
enum FMOD_OPENSTATE {
    FMOD_OPENSTATE_READY = 0,
    FMOD_OPENSTATE_LOADING = 1,
    FMOD_OPENSTATE_ERROR = 2,
    FMOD_OPENSTATE_CONNECTING = 3,
    FMOD_OPENSTATE_BUFFERING = 4,
    FMOD_OPENSTATE_SEEKING = 5,
    FMOD_OPENSTATE_STREAMING = 6,
    FMOD_OPENSTATE_SETPOSITION = 7,
    FMOD_OPENSTATE_MAX = 8,
};

/* ---- FMOD::Sound (external SDK class; opaque, only getOpenState used) ---- */
namespace FMOD {
    class Sound {
    public:
        FMOD_RESULT getOpenState(FMOD_OPENSTATE *openstate, unsigned int *percentbuffered,
                                 int *starving, int *diskbusy); // percentbuffered/diskbusy passed null here
    };
}

/* ---- ws-engine helpers the two functions call ---- */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

#include "../dbg/STRONG_ASSERT_DUMMY.h" // canonical (member + static Crash overloads) — avoids C2011

struct osTIMER {                    // DB-verified (types_members osTIMER): start@0, time@4 — size 8
    int start;                      // 0x00
    int time;                       // 0x04
    osTIMER(int start);
};

extern void osSleep(int ms);
extern "C" void osOutputDebugString(const char *fmt, ...);

/* the two boundary helpers this header serves */
extern const char *FModErrorDesc(FMOD_RESULT res, bool info);
extern void WaitSoundBank(FMOD::Sound *sound);
