#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
// Boundary declarations for the third-party FMOD Ex SDK (Firelight Technologies) and the
// small set of ws-engine `snd::` globals HALO_SOUND_SYSTEM reads directly. FMOD itself is an
// external SDK — not reversed here, only declared enough to type call sites. `snd::` globals
// are declared as externs pointing at their DB names; the objects they reference (snd::System,
// snd::SystemFMod) belong to the separate low-level snd:: driver layer (see src/ws/snd/),
// not to HALO_SOUND_SYSTEM itself.

#include <stdint.h>
#include "../ds/dsTSTRING.h"
#include "SYSTEM.h" // canonical snd::SYSTEM home for the shared `snd::System` global (2026-07-31)
// Canonical homes for the types this header used to define a SECOND time (2026-08-07 odr_dup):
//   fmod_error_boundary.h -> FMOD_RESULT, FMOD_OPENSTATE, FMOD::Sound, FMOD_TIMEUNIT, and the
//                            C++-linkage decls of FModErrorDesc / WaitSoundBank
//   SYSTEM_CUSTOM.h       -> snd::HRTF, snd::INIT, snd::SYSTEM_CUSTOM (+ SYSTEM_CUSTOM_vtbl)
//   SYSTEM_FMOD.h         -> snd::SYSTEM_DBG_FMOD (DB-exact, 1244 bytes)
// Each copy here was the narrower one; see the notes at the deletion sites in those headers.
#include "fmod_error_boundary.h"
#include "SYSTEM_CUSTOM.h"
#include "SYSTEM_FMOD.h"

// ---- FMOD_SOUND_TYPE (types_enum_values FMOD_SOUND_TYPE; only the member used) ----
// -- added by the HALO_SOUND_LIST cluster --
enum FMOD_SOUND_TYPE {
    FMOD_SOUND_TYPE_FSB = 8,
};

// ---- FMOD_DELAYTYPE (types_enum_values FMOD_DELAYTYPE) ----
enum FMOD_DELAYTYPE {
    FMOD_DELAYTYPE_END_MS = 0,
    FMOD_DELAYTYPE_DSPCLOCK_START = 1,
    FMOD_DELAYTYPE_DSPCLOCK_END = 2,
    FMOD_DELAYTYPE_DSPCLOCK_PAUSE = 3,
};

// FMOD_MODE bit. Consumers use the NAME (the one `8u` literal call site was replaced 2026-08-06).
#define FMOD_2D 0x00000008
// Reserved channel handle meaning "let FMOD pick any free channel".
#define FMOD_CHANNEL_FREE (-1)
// -- FMOD_MODE bits added by the HALO_SOUND_LIST cluster (CreateSound's raw open-mode flags) --
#define FMOD_OPENMEMORY_POINT       0x10000000
#define FMOD_CREATECOMPRESSEDSAMPLE 0x00000200
#define FMOD_NONBLOCKING            0x00010000

// FMOD_VECTOR (types_members FMOD_VECTOR) -- plain 3-float vector. Added by the HALO_CHANNEL
// cluster (Channel::set3DAttributes/set3DConeOrientation).
typedef struct FMOD_VECTOR {
    float x; // 0x00
    float y; // 0x04
    float z; // 0x08
} FMOD_VECTOR;

// The real FMOD_CREATESOUNDEXINFO is defined at global scope further down; forward-declare it here
// so the createSound/createStream members below name THAT type (::FMOD_CREATESOUNDEXINFO) rather
// than accidentally forward-declaring a distinct incomplete FMOD::FMOD_CREATESOUNDEXINFO.
struct FMOD_CREATESOUNDEXINFO;

namespace FMOD {

// class Sound is defined in fmod_error_boundary.h (included above).
struct Channel;
struct ChannelGroup;
struct DSP;
struct DSPConnection;

typedef struct System {
    FMOD_RESULT getSystemObject(System **system);                                       // boundary — FMOD SDK
    FMOD_RESULT getDSPClock(unsigned int *hi, unsigned int *lo);                         // boundary — FMOD SDK
    FMOD_RESULT getDSPBufferSize(unsigned int *bufferLength, int *numBuffers);           // boundary — FMOD SDK
    FMOD_RESULT getSoftwareFormat(int *sampleRate, int *format, int *numOutputChannels,
                                  int *maxInputChannels, int *resample, int *bits);      // boundary — FMOD SDK
    FMOD_RESULT playSound(int channelIndex, Sound *sound, bool paused, Channel **channel); // boundary — FMOD SDK
    FMOD_RESULT createDSP(const struct FMOD_DSP_DESCRIPTION *description, DSP **dsp);      // boundary — FMOD SDK (HALO_CROSSFADE_DSP cluster)
    // -- added by the HALO_SOUND_LIST/HALO_CHANNEL cluster --
    FMOD_RESULT createSound(const char *name_or_data, unsigned int mode,
                             ::FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);        // boundary — FMOD SDK
    FMOD_RESULT createStream(const char *name_or_data, unsigned int mode,
                              ::FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);       // boundary — FMOD SDK
    FMOD_RESULT getChannelsPlaying(int *channels);                                         // boundary — FMOD SDK
} System;

// ChannelGroup -- a mixing bus (snd::SystemFMod->master/->reverb). Added by the HALO_CHANNEL
// cluster; only ever passed by pointer, full API not modeled.
typedef struct ChannelGroup {
} ChannelGroup;

typedef struct Channel {
    FMOD_RESULT stop();                                                                 // boundary — FMOD SDK
    FMOD_RESULT setDelay(FMOD_DELAYTYPE type, unsigned int hi, unsigned int lo);         // boundary — FMOD SDK
    FMOD_RESULT setMode(unsigned int mode);                                             // boundary — FMOD SDK
    FMOD_RESULT setVolume(float volume);                                                // boundary — FMOD SDK
    FMOD_RESULT setPaused(bool paused);                                                 // boundary — FMOD SDK
    FMOD_RESULT isPlaying(bool *isPlaying);                                             // boundary — FMOD SDK
    // -- added by the HALO_PERM_SOUND/HALO_CROSSFADE_DSP cluster --
    FMOD_RESULT getCurrentSound(Sound **sound);                                          // boundary — FMOD SDK
    FMOD_RESULT getFrequency(float *frequency);                                          // boundary — FMOD SDK
    FMOD_RESULT addDSP(DSP *dsp, DSPConnection **connection);                            // boundary — FMOD SDK
    FMOD_RESULT getDelay(FMOD_DELAYTYPE delaytype, unsigned int *delayhi, unsigned int *delaylo); // boundary — FMOD SDK
    FMOD_RESULT getPosition(unsigned int *position, FMOD_TIMEUNIT postype);              // boundary — FMOD SDK
    // -- added by the HALO_SOUND_LIST/HALO_CHANNEL cluster --
    FMOD_RESULT setChannelGroup(ChannelGroup *channelgroup);                              // boundary — FMOD SDK
    FMOD_RESULT set3DAttributes(const FMOD_VECTOR *pos, const FMOD_VECTOR *vel);          // boundary — FMOD SDK
    FMOD_RESULT set3DConeOrientation(FMOD_VECTOR *orientation);                           // boundary — FMOD SDK
    FMOD_RESULT set3DOcclusion(float directocclusion, float reverbocclusion);             // boundary — FMOD SDK
    FMOD_RESULT set3DPanLevel(float level);                                               // boundary — FMOD SDK
    FMOD_RESULT set3DConeSettings(float insideconeangle, float outsideconeangle,
                                   float outsidevolume);                                   // boundary — FMOD SDK
    FMOD_RESULT set3DMinMaxDistance(float mindistance, float maxdistance);                 // boundary — FMOD SDK
    FMOD_RESULT set3DSpread(float angle);                                                  // boundary — FMOD SDK
    FMOD_RESULT setSpeakerMix(float frontleft, float frontright, float center, float lfe,
                               float backleft, float backright, float sideleft,
                               float sideright);                                          // boundary — FMOD SDK
    FMOD_RESULT setFrequency(float frequency);                                             // boundary — FMOD SDK
} Channel;

// DSP -- added by the HALO_CROSSFADE_DSP cluster (custom FMOD DSP unit wrapper).
typedef struct DSP {
    FMOD_RESULT setBypass(bool bypass);                    // boundary — FMOD SDK
    FMOD_RESULT disconnectAll(bool inputs, bool outputs);  // boundary — FMOD SDK
    FMOD_RESULT getUserData(void **userdata);              // boundary — FMOD SDK
    FMOD_RESULT getSystemObject(System **system);          // boundary — FMOD SDK
} DSP;

} // namespace FMOD

// FMOD_DSP_STATE -- DB-verified layout (types_members FMOD_DSP_STATE). Added by the
// HALO_CROSSFADE_DSP cluster (custom DSP read/release callbacks).
typedef struct FMOD_DSP_STATE {
    FMOD::DSP        *instance;    // 0x0
    void             *plugindata;  // 0x4
    uint16_t  speakermask; // 0x8
} FMOD_DSP_STATE;

// FMOD_DSP_PARAMETERDESC -- referenced only by pointer (paramdesc); not reversed.
struct FMOD_DSP_PARAMETERDESC;
// The C API's opaque sound handle. FMOD_CREATESOUNDEXINFO is the C struct, so its three sound
// callbacks take FMOD_SOUND * (types_members FMOD_CREATESOUNDEXINFO agrees) -- NOT the C++ wrapper
// FMOD::Sound *, which is what these three slots used to say. FMOD SDK boundary: no layout.
struct FMOD_SOUND;

// FMOD_CREATESOUNDEXINFO (types_members FMOD_CREATESOUNDEXINFO) -- 112 bytes. Added by the
// HALO_SOUND_LIST cluster (CreateSound only sets cbsize/length/suggestedsoundtype; every
// other field is zero-initialized by the caller and passed through untouched).
typedef struct FMOD_CREATESOUNDEXINFO {
    int                 cbsize;              // 0x00
    unsigned int        length;              // 0x04
    unsigned int        fileoffset;          // 0x08
    int                 numchannels;         // 0x0C
    int                 defaultfrequency;    // 0x10
    int                 format;              // 0x14 FMOD_SOUND_FORMAT
    unsigned int        decodebuffersize;    // 0x18
    int                 initialsubsound;     // 0x1C
    int                 numsubsounds;        // 0x20
    int                *inclusionlist;       // 0x24
    int                 inclusionlistnum;    // 0x28
    FMOD_RESULT       (*pcmreadcallback)(FMOD_SOUND *, void *, unsigned int);                  // 0x2C
    FMOD_RESULT       (*pcmsetposcallback)(FMOD_SOUND *, int, unsigned int, unsigned int); // 0x30
    FMOD_RESULT       (*nonblockcallback)(FMOD_SOUND *, FMOD_RESULT);                           // 0x34
    const char         *dlsname;             // 0x38
    const char         *encryptionkey;       // 0x3C
    int                 maxpolyphony;        // 0x40
    void               *userdata;            // 0x44
    FMOD_SOUND_TYPE     suggestedsoundtype;  // 0x48
    FMOD_RESULT       (*useropen)(const char *, int, unsigned int *, void **, void **); // 0x4C
    FMOD_RESULT       (*userclose)(void *, void *);                                                // 0x50
    FMOD_RESULT       (*userread)(void *, void *, unsigned int, unsigned int *, void *); // 0x54
    FMOD_RESULT       (*userseek)(void *, unsigned int, void *);                                    // 0x58
    int                 speakermap;          // 0x5C FMOD_SPEAKERMAPTYPE
    struct FMOD_SOUNDGROUP *initialsoundgroup; // 0x60
    unsigned int        initialseekposition; // 0x64
    unsigned int        initialseekpostype;  // 0x68
    int                  ignoresetfilesystem;// 0x6C
} FMOD_CREATESOUNDEXINFO; // 112 bytes (cbsize == 112, matches sizeof at this FMOD Ex version)

// WaitSoundBank (blocks until an FMOD_NONBLOCKING sound bank finishes opening) is declared by
// fmod_error_boundary.h, which also owns its definition's TU. It was declared `extern "C"` here;
// the DB names it ?WaitSoundBank@@YAXPAVSound@FMOD@@@Z, i.e. C++ linkage, so the two declarations
// could never have referred to the same symbol.

// FMOD_DSP_DESCRIPTION -- DB-verified layout (types_members FMOD_DSP_DESCRIPTION), 92 bytes.
// Added by the HALO_CROSSFADE_DSP cluster (custom DSP creation descriptor).
typedef struct FMOD_DSP_DESCRIPTION {
    char                    name[32];        // 0x00
    unsigned int            version;         // 0x20
    int                     channels;        // 0x24
    FMOD_RESULT (*create)(FMOD_DSP_STATE *);                                                   // 0x28
    FMOD_RESULT (*release)(FMOD_DSP_STATE *);                                                  // 0x2C
    FMOD_RESULT (*reset)(FMOD_DSP_STATE *);                                                    // 0x30
    FMOD_RESULT (*read)(FMOD_DSP_STATE *, float *, float *, unsigned int, int, int); // 0x34
    FMOD_RESULT (*setposition)(FMOD_DSP_STATE *, unsigned int);                                 // 0x38
    int                     numparameters;   // 0x3C
    FMOD_DSP_PARAMETERDESC *paramdesc;       // 0x40
    FMOD_RESULT (*setparameter)(FMOD_DSP_STATE *, int, float);                                  // 0x44
    FMOD_RESULT (*getparameter)(FMOD_DSP_STATE *, int, float *, char *);                        // 0x48
    FMOD_RESULT (*config)(FMOD_DSP_STATE *, void *, int);                                       // 0x4C
    int                     configwidth;     // 0x50
    int                     configheight;    // 0x54
    void                   *userdata;        // 0x58
} FMOD_DSP_DESCRIPTION;

// FModErrorDesc (human-readable FMOD_RESULT string; the second param is observed always 0 at
// HALO_SOUND_SYSTEM call sites) is declared by fmod_error_boundary.h. Same story as WaitSoundBank:
// it was `extern "C"` here directly under a comment quoting its own C++-mangled DB name
// ?FModErrorDesc@@YAPBDW4FMOD_RESULT@@_N@Z.

namespace snd {

// dsFLAGS-wrapped live/dead state bit 0 checked as `snd::System->state.val & 1`.
typedef struct SYSTEM_STATE_FLAGS { int val; } SYSTEM_STATE_FLAGS;

// Minimal boundary view of the low-level snd:: driver-state singleton (full layout owned by
// the separate src/ws/snd/ drain) — only the one field HALO_SOUND_SYSTEM reads.
typedef struct SYSTEM_BASE {
    SYSTEM_STATE_FLAGS state; // offset unresolved beyond this single bit test; boundary
} SYSTEM_BASE;
// Canonical snd::SYSTEM* (from SYSTEM.h) so this decl agrees with the full-type home when both
// headers meet in one TU; snd::SYSTEM::state is a dsFLAGS with the same .val bit-0 view. (2026-07-31)
extern SYSTEM *System; // ?System@snd@@3PAVSYSTEM@@... — boundary global

// SYSTEM_DBG_FMOD is defined by SYSTEM_FMOD.h (included above) at its DB-exact 1244 bytes. The
// three-member view that lived here put `disableNonblocking` at offset 0, where the DB has
// `enableStats` — the flag HALO_SOUND_LIST::CreateSound tests was reading the wrong byte.

// FMOD backend singleton; only ->system (the FMOD::System*) and ->dbg are used here.
// NOTE: SYSTEM_FMOD_BASE is a compile-only stand-in and is NOT a DB type — the real singleton is
// snd::SYSTEM_FMOD (SYSTEM_FMOD.h, 2320 bytes: system@52, master@56, reverb@60, dbg@0x430). Only
// the member NAMES below are evidence; their offsets here are not. Left as-is by the odr_dup pass.
typedef struct SYSTEM_FMOD_BASE {
    FMOD::System *system; // boundary offset — owned by src/ws/snd/ drain
    SYSTEM_DBG_FMOD dbg;   // boundary offset
    // -- added by the HALO_SOUND_LIST/HALO_CHANNEL cluster --
    SYSTEM_STATE_FLAGS  state;  // boundary offset — bit9 forces a blocking sound open (see CreateSound)
    FMOD::ChannelGroup *master; // boundary offset — default (non-reverb) mixer bus
    FMOD::ChannelGroup *reverb; // boundary offset — reverb-send mixer bus
} SYSTEM_FMOD_BASE;
extern SYSTEM_FMOD_BASE *SystemFMod; // boundary global

extern int THREAD_ID;                      // owning-thread id asserted by every public method (?THREAD_ID@snd@@3HA, int)
extern dsTSTRING<char> CUSTOM_SOUND_PATH;  // "d:\sounds\xbox360\" base path for custom sounds
extern dsTSTRING<char> SOUND_LOCALE;       // active locale code (e.g. "en")

// snd::HRTF, snd::INIT, snd::SYSTEM_CUSTOM and SYSTEM_CUSTOM_vtbl are defined by SYSTEM_CUSTOM.h
// (included above), which is the only one of the two copies that spelled INIT's 0x03 pad and the
// vtbl's seven slots. HALO_SOUND_SYSTEM derives from SYSTEM_CUSTOM and takes an INIT by value in
// Init(), so the layouts have to be the ones the DB gives: INIT is 8 bytes with hrtf@4, and
// SYSTEM_CUSTOM is 4 bytes with __vftable@0.

} // namespace snd
