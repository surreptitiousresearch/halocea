#pragma once
// HCEX sound bridge: one virtual playback channel — element type of HALO_SOUND_SYSTEM::
// channels / playToEnd. Wraps a "current" and a "queued" HALO_PERM_SOUND slot plus the
// 3D/2D playback properties last submitted for it.
// DB-verified layout (types_members HALO_CHANNEL): state@0 (HALO_CHANNEL::STATE),
// index@4 (const int), current@8 (HALO_PERM_SOUND, 60 bytes), queued@68 (HALO_PERM_SOUND,
// 60 bytes), props@128 (HALO_CHANNEL_PROPERTIES, 80 bytes) — size 208.
// Source: D:\Projects\code\HCEX\sources\sound\fmod\sound_dsound_fmod.cpp/.h.

#include "HALO_PERM_SOUND.h"
#include "HALO_CHANNEL_PROPERTIES.h"

struct sound_location;      // Blam boundary (../sound_location.h)
struct sound_permutation;   // Blam boundary (../sound_permutation.h)
struct platform_sound_channel_properties; // Blam boundary (../platform_sound_channel_properties.h)

// types_enum_values HALO_CHANNEL::STATE.
enum HALO_CHANNEL_STATE {
    ST_IDLE               = 0, // nothing bound; current/queued both invalid
    ST_PLAYING            = 1, // current is the only sound playing
    ST_PLAYING_HAS_QUEUED = 2, // current is playing and queued is waiting to replace it
    ST_PLAYING_TO_END     = 3, // current is finishing on its own (params->PLAY_TO_END); no queued allowed
    ST_CROSSFADE          = 4, // current (outgoing) and queued (incoming) are both live, crossfading
};

typedef struct HALO_CHANNEL {
    // Re-exported under its DB name so the field type below matches the mangled member type.
    typedef HALO_CHANNEL_STATE STATE;

    STATE                       state;   // 0x00
    const int                    index;   // 0x04 -- this channel's slot index in haloSoundSystem->channels
    HALO_PERM_SOUND               current; // 0x08 -- the sound actually bound to an FMOD::Channel
    HALO_PERM_SOUND               queued;  // 0x44 -- the next sound waiting to take over, or invalid (fsbIndex==-1)
    HALO_CHANNEL_PROPERTIES       props;   // 0x80 -- shared 3D/2D playback properties for both current/queued

    HALO_CHANNEL(int index); // ??0HALO_CHANNEL@@QAA@H@Z
    ~HALO_CHANNEL();          // ??1HALO_CHANNEL@@QAA@XZ
    HALO_CHANNEL *scalarDeletingDtor(unsigned char deleteFlags); // ??_GHALO_CHANNEL@@QAAPAXI@Z

    // Bind `sound` to this channel (borrowing a matching still-loading preloadList entry's FMOD
    // banks if one exists, else HALO_SOUND_LIST::GetFreeSound), carry over `previous`'s FMOD
    // channel for stitching if given, then CheckNotReady to actually start playback once ready.
    void Play(HALO_PERM_SOUND *sound, HALO_PERM_SOUND *previous); // ?Play@HALO_CHANNEL@@AAAXAAUHALO_PERM_SOUND@@PAU2@@Z

    // Poll/advance this channel's state machine: finish opening `current`/`queued`'s FMOD
    // banks (CheckNotReady), detect `current` finishing (isPlaying()==false) and either advance
    // to a queued sound, hand off to ST_PLAYING_TO_END, or go idle; promote ST_PLAYING_HAS_QUEUED
    // into an actual ST_CROSSFADE once queued is ready to play.
    void Update(); // ?Update@HALO_CHANNEL@@QAAXXZ

    // current's/queued's playback state as a simple tri-state for callers that don't need the
    // full STATE machine: 0 = idle, 1 = playing (normally or to-end), 2 = queued/crossfading.
    // Runs Update() first to make sure `state` is current.
    int GetState(); // ?GetState@HALO_CHANNEL@@QAAHXZ

    // Stop both current and queued immediately (HALO_PERM_SOUND::Stop tears down their FMOD
    // channels/banks and resets each to invalid).
    void Stop(); // ?Stop@HALO_CHANNEL@@QAAXXZ

    // Queue `perm`/`identifier` to play next: if idle, starts it immediately via Play(); if
    // already playing, waits in `queued` (promoting ST_PLAYING to ST_PLAYING_HAS_QUEUED) for
    // Update() to crossfade into once ready. A no-op while ST_CROSSFADE is in progress.
    void Queue(const sound_permutation *perm, int identifier); // ?Queue@HALO_CHANNEL@@QAAXPBUsound_permutation@@J@Z

    // Update props (3D position/orientation/velocity, occlusion/obstruction, underwater, and
    // the is3D/spatialize flag) and push them to whichever of current/queued has a live FMOD
    // channel via UpdateLocation.
    void SetLocation(bool spatialize, const sound_location *location, float occlusion,
                      float obstruction, bool underwater); // ?SetLocation@HALO_CHANNEL@@QAAX_NPBUsound_location@@MM0@Z

    // Update props' volume/pitch/cone/attenuation fields from `properties` (skipping everything
    // but volume when gainOnly), recompute distanceToPlayer, then push to whichever of current/
    // queued has a live FMOD channel via UpdateProperties(initial=false).
    void SetProperties(const platform_sound_channel_properties *properties, bool gainOnly,
                         int class_index); // ?SetProperties@HALO_CHANNEL@@QAAXPBUplatform_sound_channel_properties@@_NH@Z

private:
    // Finish standing up `sound`'s FMOD playback: if its channel already exists, only re-syncs
    // playback frequency after a stitch (loop-point) change; otherwise waits for the sound bank
    // to finish opening, resolves its sub-sound, starts FMOD::System::playSound (stealing an old
    // channel first if FMOD is at its 32-channel cap), assigns the channel group (master/reverb),
    // pushes initial location+properties, and fires the debug "PLAY ..." log line. Returns true
    // while still waiting on the bank/sub-sound to open (caller should retry next Update()).
    bool CheckNotReady(HALO_PERM_SOUND *sound); // ?CheckNotReady@HALO_CHANNEL@@AAA_NAAUHALO_PERM_SOUND@@@Z

    // Push props' 3D position/velocity/orientation/occlusion to `snd`'s live FMOD channel
    // (only when props.is3D), plus its pan-level remap once HALO_SOUND_HAS_DIST_LT-style
    // distance params are present on `snd.params`.
    void UpdateLocation(HALO_PERM_SOUND *snd); // ?UpdateLocation@HALO_CHANNEL@@QAAXAAUHALO_PERM_SOUND@@@Z

    // Push props' volume/pitch/cone/attenuation/speaker-mix to `snd`'s live FMOD channel.
    // `initial` additionally sets the channel's 2D/3D mode, cone settings, min/max distance,
    // spread, and (dialog/music category) explicit speaker mix -- done once per Play(), not on
    // every SetProperties() update.
    void UpdateProperties(HALO_PERM_SOUND *snd, bool initial); // ?UpdateProperties@HALO_CHANNEL@@QAAXAAUHALO_PERM_SOUND@@_N@Z
} HALO_CHANNEL;
