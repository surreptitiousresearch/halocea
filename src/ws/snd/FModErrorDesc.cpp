// FModErrorDesc @0x831BCE98 — map an FMOD_RESULT to a printable string: the enum-name spelling
// (info==false) or the verbose human description (info==true). Boundary strings verbatim from the
// binary. See headers/ws/snd/fmod_error_boundary.h for the FMOD boundary declarations.
#include "../headers/ws/snd/fmod_error_boundary.h"

const char *FModErrorDesc(FMOD_RESULT res, bool info)
{
    const char *result;

    // On corrupt/mismatched sound data the caller-facing assert fires before the lookup.
    if (res == FMOD_ERR_EVENT_MISMATCH && !IGNORE_STRONG_ASSERT)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("Invalid sound data. Get latest version or rebuild sounds.",
            "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd_buf_fmod.cpp", 1318, empty_string);

    switch (res)
    {
    case FMOD_OK:
        result = info ? "No errors."
                      : "FMOD_OK";
        break;
    case FMOD_ERR_ALREADYLOCKED:
        result = info ? "Tried to call lock a second time before unlock was called."
                      : "FMOD_ERR_ALREADYLOCKED";
        break;
    case FMOD_ERR_BADCOMMAND:
        result = info ? "Tried to call a function on a data type that does not allow this type of functionality (ie calling Soun" "d::lock on a streaming sound)."
                      : "FMOD_ERR_BADCOMMAND";
        break;
    case FMOD_ERR_CDDA_DRIVERS:
        result = info ? "Neither NTSCSI nor ASPI could be initialised."
                      : "FMOD_ERR_CDDA_DRIVERS";
        break;
    case FMOD_ERR_CDDA_INIT:
        result = info ? "An error occurred while initialising the CDDA subsystem."
                      : "FMOD_ERR_CDDA_INIT";
        break;
    case FMOD_ERR_CDDA_INVALID_DEVICE:
        result = info ? "Couldn't find the specified device."
                      : "FMOD_ERR_CDDA_INVALID_DEVICE";
        break;
    case FMOD_ERR_CDDA_NOAUDIO:
        result = info ? "No audio tracks on the specified disc."
                      : "FMOD_ERR_CDDA_NOAUDIO";
        break;
    case FMOD_ERR_CDDA_NODEVICES:
        result = info ? "No CD/DVD devices were found."
                      : "FMOD_ERR_CDDA_NODEVICES";
        break;
    case FMOD_ERR_CDDA_NODISC:
        result = info ? "No disc present in the specified drive."
                      : "FMOD_ERR_CDDA_NODISC";
        break;
    case FMOD_ERR_CDDA_READ:
        result = info ? "A CDDA read error occurred."
                      : "FMOD_ERR_CDDA_READ";
        break;
    case FMOD_ERR_CHANNEL_ALLOC:
        result = info ? "Error trying to allocate a channel."
                      : "FMOD_ERR_CHANNEL_ALLOC";
        break;
    case FMOD_ERR_CHANNEL_STOLEN:
        result = info ? "The specified channel has been reused to play another sound."
                      : "FMOD_ERR_CHANNEL_STOLEN";
        break;
    case FMOD_ERR_COM:
        result = info ? "A Win32 COM related error occured. COM failed to initialize or a QueryInterface failed meaning a Window" "s codec or driver was not installed properly."
                      : "FMOD_ERR_COM";
        break;
    case FMOD_ERR_DMA:
        result = info ? "DMA Failure.  See debug output for more information."
                      : "FMOD_ERR_DMA";
        break;
    case FMOD_ERR_DSP_CONNECTION:
        result = info ? "DSP connection error.  Connection possibly caused a cyclic dependancy."
                      : "FMOD_ERR_DSP_CONNECTION";
        break;
    case FMOD_ERR_DSP_FORMAT:
        result = info ? "DSP Format error.  A DSP unit may have attempted to connect to this network with the wrong format."
                      : "FMOD_ERR_DSP_FORMAT";
        break;
    case FMOD_ERR_DSP_NOTFOUND:
        result = info ? "DSP connection error.  Couldn't find the DSP unit specified."
                      : "FMOD_ERR_DSP_NOTFOUND";
        break;
    case FMOD_ERR_DSP_RUNNING:
        result = info ? "DSP error.  Cannot perform this operation while the network is in the middle of running.  This will mos" "t likely happen if a connection or disconnection is attempted in a DSP callback."
                      : "FMOD_ERR_DSP_RUNNING";
        break;
    case FMOD_ERR_DSP_TOOMANYCONNECTIONS:
        result = info ? "DSP connection error.  The unit being connected to or disconnected should only have 1 input or output."
                      : "FMOD_ERR_DSP_TOOMANYCONNECTIONS";
        break;
    case FMOD_ERR_FILE_BAD:
        result = info ? "Error loading file."
                      : "FMOD_ERR_FILE_BAD";
        break;
    case FMOD_ERR_FILE_COULDNOTSEEK:
        result = info ? "Couldn't perform seek operation.  This is a limitation of the medium (ie netstreams) or the file format."
                      : "FMOD_ERR_FILE_COULDNOTSEEK";
        break;
    case FMOD_ERR_FILE_DISKEJECTED:
        result = info ? "Media was ejected while reading."
                      : "FMOD_ERR_FILE_DISKEJECTED";
        break;
    case FMOD_ERR_FILE_EOF:
        result = info ? "End of file unexpectedly reached while trying to read essential data (truncated data?)."
                      : "FMOD_ERR_FILE_EOF";
        break;
    case FMOD_ERR_FILE_NOTFOUND:
        result = info ? "File not found."
                      : "FMOD_ERR_FILE_NOTFOUND";
        break;
    case FMOD_ERR_FILE_UNWANTED:
        result = info ? "Unwanted file access occured."
                      : "FMOD_ERR_FILE_UNWANTED";
        break;
    case FMOD_ERR_FORMAT:
        result = info ? "Unsupported file or audio format."
                      : "FMOD_ERR_FORMAT";
        break;
    case FMOD_ERR_HTTP:
        result = info ? "A HTTP error occurred. This is a catch-all for HTTP errors not listed elsewhere."
                      : "FMOD_ERR_HTTP";
        break;
    case FMOD_ERR_HTTP_ACCESS:
        result = info ? "The specified resource requires authentication or is forbidden."
                      : "FMOD_ERR_HTTP_ACCESS";
        break;
    case FMOD_ERR_HTTP_PROXY_AUTH:
        result = info ? "Proxy authentication is required to access the specified resource."
                      : "FMOD_ERR_HTTP_PROXY_AUTH";
        break;
    case FMOD_ERR_HTTP_SERVER_ERROR:
        result = info ? "A HTTP server error occurred."
                      : "FMOD_ERR_HTTP_SERVER_ERROR";
        break;
    case FMOD_ERR_HTTP_TIMEOUT:
        result = info ? "The HTTP request timed out."
                      : "FMOD_ERR_HTTP_TIMEOUT";
        break;
    case FMOD_ERR_INITIALIZATION:
        result = info ? "FMOD was not initialized correctly to support this function."
                      : "FMOD_ERR_INITIALIZATION";
        break;
    case FMOD_ERR_INITIALIZED:
        result = info ? "Cannot call this command after System::init."
                      : "FMOD_ERR_INITIALIZED";
        break;
    case FMOD_ERR_INTERNAL:
        result = info ? "An error occured that wasnt supposed to.  Contact support."
                      : "FMOD_ERR_INTERNAL";
        break;
    case FMOD_ERR_INVALID_ADDRESS:
        result = info ? "On Xbox 360, this memory address passed to FMOD must be physical, (ie allocated with XPhysicalAlloc.)"
                      : "FMOD_ERR_INVALID_ADDRESS";
        break;
    case FMOD_ERR_INVALID_FLOAT:
        result = info ? "Value passed in was a NaN, Inf or denormalized float."
                      : "FMOD_ERR_INVALID_FLOAT";
        break;
    case FMOD_ERR_INVALID_HANDLE:
        result = info ? "An invalid object handle was used."
                      : "FMOD_ERR_INVALID_HANDLE";
        break;
    case FMOD_ERR_INVALID_PARAM:
        result = info ? "An invalid parameter was passed to this function."
                      : "FMOD_ERR_INVALID_PARAM";
        break;
    case FMOD_ERR_INVALID_POSITION:
        result = info ? "An invalid seek position was passed to this function."
                      : "FMOD_ERR_INVALID_POSITION";
        break;
    case FMOD_ERR_INVALID_SPEAKER:
        result = info ? "An invalid speaker was passed to this function based on the current speaker mode."
                      : "FMOD_ERR_INVALID_SPEAKER";
        break;
    case FMOD_ERR_INVALID_SYNCPOINT:
        result = info ? "The syncpoint did not come from this sound handle."
                      : "FMOD_ERR_INVALID_SYNCPOINT";
        break;
    case FMOD_ERR_INVALID_VECTOR:
        result = info ? "The vectors passed in are not unit length, or perpendicular."
                      : "FMOD_ERR_INVALID_VECTOR";
        break;
    case FMOD_ERR_IRX:
        result = info ? "PS2 only.  fmodex.irx failed to initialize.  This is most likely because you forgot to load it."
                      : "FMOD_ERR_IRX";
        break;
    case FMOD_ERR_MAXAUDIBLE:
        result = info ? "Reached maximum audible playback count for this sound's soundgroup."
                      : "FMOD_ERR_MAXAUDIBLE";
        break;
    case FMOD_ERR_MEMORY:
        result = info ? "Not enough memory or resources."
                      : "FMOD_ERR_MEMORY";
        break;
    case FMOD_ERR_MEMORY_CANTPOINT:
        result = info ? "Can't use FMOD_OPENMEMORY_POINT on non PCM source data, or non mp3/xma/adpcm data if FMOD_CREATECOMPRES" "SEDSAMPLE was used."
                      : "FMOD_ERR_MEMORY_CANTPOINT";
        break;
    case FMOD_ERR_MEMORY_IOP:
        result = info ? "PS2 only.  Not enough memory or resources on PlayStation 2 IOP ram."
                      : "FMOD_ERR_MEMORY_IOP";
        break;
    case FMOD_ERR_MEMORY_SRAM:
        result = info ? "Not enough memory or resources on console sound ram."
                      : "FMOD_ERR_MEMORY_SRAM";
        break;
    case FMOD_ERR_NEEDS2D:
        result = info ? "Tried to call a command on a 3d sound when the command was meant for 2d sound."
                      : "FMOD_ERR_NEEDS2D";
        break;
    case FMOD_ERR_NEEDS3D:
        result = info ? "Tried to call a command on a 2d sound when the command was meant for 3d sound."
                      : "FMOD_ERR_NEEDS3D";
        break;
    case FMOD_ERR_NEEDSHARDWARE:
        result = info ? "Tried to use a feature that requires hardware support.  (ie trying to play a VAG compressed sound in software on PS2)."
                      : "FMOD_ERR_NEEDSHARDWARE";
        break;
    case FMOD_ERR_NEEDSSOFTWARE:
        result = info ? "Tried to use a feature that requires the software engine.  Software engine has either been turned off, " "or command was executed on a hardware channel which does not support this feature."
                      : "FMOD_ERR_NEEDSSOFTWARE";
        break;
    case FMOD_ERR_NET_CONNECT:
        result = info ? "Couldn't connect to the specified host."
                      : "FMOD_ERR_NET_CONNECT";
        break;
    case FMOD_ERR_NET_SOCKET_ERROR:
        result = info ? "A socket error occurred.  This is a catch-all for socket-related errors not listed elsewhere."
                      : "FMOD_ERR_NET_SOCKET_ERROR";
        break;
    case FMOD_ERR_NET_URL:
        result = info ? "The specified URL couldn't be resolved."
                      : "FMOD_ERR_NET_URL";
        break;
    case FMOD_ERR_NET_WOULD_BLOCK:
        result = info ? "Operation on a non-blocking socket could not complete immediately."
                      : "FMOD_ERR_NET_WOULD_BLOCK";
        break;
    case FMOD_ERR_NOTREADY:
        result = info ? "Operation could not be performed because specified sound/DSP connection is not ready."
                      : "FMOD_ERR_NOTREADY";
        break;
    case FMOD_ERR_OUTPUT_ALLOCATED:
        result = info ? "Error initializing output device, but more specifically, the output device is already in use and cannot be reused."
                      : "FMOD_ERR_OUTPUT_ALLOCATED";
        break;
    case FMOD_ERR_OUTPUT_CREATEBUFFER:
        result = info ? "Error creating hardware sound buffer."
                      : "FMOD_ERR_OUTPUT_CREATEBUFFER";
        break;
    case FMOD_ERR_OUTPUT_DRIVERCALL:
        result = info ? "A call to a standard soundcard driver failed, which could possibly mean a bug in the driver or resource" "s were missing or exhausted."
                      : "FMOD_ERR_OUTPUT_DRIVERCALL";
        break;
    case FMOD_ERR_OUTPUT_ENUMERATION:
        result = info ? "Error enumerating the available driver list. List may be inconsistent due to a recent device addition or removal."
                      : "FMOD_ERR_OUTPUT_ENUMERATION";
        break;
    case FMOD_ERR_OUTPUT_FORMAT:
        result = info ? "Soundcard does not support the minimum features needed for this soundsystem (16bit stereo output)."
                      : "FMOD_ERR_OUTPUT_FORMAT";
        break;
    case FMOD_ERR_OUTPUT_INIT:
        result = info ? "Error initializing output device."
                      : "FMOD_ERR_OUTPUT_INIT";
        break;
    case FMOD_ERR_OUTPUT_NOHARDWARE:
        result = info ? "FMOD_HARDWARE was specified but the sound card does not have the resources nescessary to play it."
                      : "FMOD_ERR_OUTPUT_NOHARDWARE";
        break;
    case FMOD_ERR_OUTPUT_NOSOFTWARE:
        result = info ? "Attempted to create a software sound but no software channels were specified in System::init."
                      : "FMOD_ERR_OUTPUT_NOSOFTWARE";
        break;
    case FMOD_ERR_PAN:
        result = info ? "Panning only works with mono or stereo sound sources."
                      : "FMOD_ERR_PAN";
        break;
    case FMOD_ERR_PLUGIN:
        result = info ? "An unspecified error has been returned from a 3rd party plugin."
                      : "FMOD_ERR_PLUGIN";
        break;
    case FMOD_ERR_PLUGIN_INSTANCES:
        result = info ? "The number of allowed instances of a plugin has been exceeded."
                      : "FMOD_ERR_PLUGIN_INSTANCES";
        break;
    case FMOD_ERR_PLUGIN_MISSING:
        result = info ? "A requested output, dsp unit type or codec was not available."
                      : "FMOD_ERR_PLUGIN_MISSING";
        break;
    case FMOD_ERR_PLUGIN_RESOURCE:
        result = info ? "A resource that the plugin requires cannot be found. (ie the DLS file for MIDI playback)"
                      : "FMOD_ERR_PLUGIN_RESOURCE";
        break;
    case FMOD_ERR_RECORD:
        result = info ? "An error occured trying to initialize the recording device."
                      : "FMOD_ERR_RECORD";
        break;
    case FMOD_ERR_REVERB_INSTANCE:
        result = info ? "Specified Instance in FMOD_REVERB_PROPERTIES couldn't be set. Most likely because another application h" "as locked the EAX4 FX slot."
                      : "FMOD_ERR_REVERB_INSTANCE";
        break;
    case FMOD_ERR_SUBSOUND_ALLOCATED:
        result = info ? "This subsound is already being used by another sound, you cannot have more than one parent to a sound. " " Null out the other parent's entry first."
                      : "FMOD_ERR_SUBSOUND_ALLOCATED";
        break;
    case FMOD_ERR_SUBSOUND_CANTMOVE:
        result = info ? "Shared subsounds cannot be replaced or moved from their parent stream, such as when the parent stream is an FSB file."
                      : "FMOD_ERR_SUBSOUND_CANTMOVE";
        break;
    case FMOD_ERR_SUBSOUND_MODE:
        result = info ? "The subsound's mode bits do not match with the parent sound's mode bits.  See documentation for functio" "n that it was called with."
                      : "FMOD_ERR_SUBSOUND_MODE";
        break;
    case FMOD_ERR_SUBSOUNDS:
        result = info ? "The error occured because the sound referenced contains subsounds.  (ie you cannot play the parent soun" "d as a static sample, only its subsounds.)"
                      : "FMOD_ERR_SUBSOUNDS";
        break;
    case FMOD_ERR_TAGNOTFOUND:
        result = info ? "The specified tag could not be found or there are no tags."
                      : "FMOD_ERR_TAGNOTFOUND";
        break;
    case FMOD_ERR_TOOMANYCHANNELS:
        result = info ? "The sound created exceeds the allowable input channel count.  This can be increased using the maxinputc" "hannels parameter in System::setSoftwareFormat."
                      : "FMOD_ERR_TOOMANYCHANNELS";
        break;
    case FMOD_ERR_UNIMPLEMENTED:
        result = info ? "Something in FMOD hasn't been implemented when it should be! contact support!"
                      : "FMOD_ERR_UNIMPLEMENTED";
        break;
    case FMOD_ERR_UNINITIALIZED:
        result = info ? "This command failed because System::init or System::setDriver was not called."
                      : "FMOD_ERR_UNINITIALIZED";
        break;
    case FMOD_ERR_UNSUPPORTED:
        result = info ? "A command issued was not supported by this object.  Possibly a plugin without certain callbacks specified."
                      : "FMOD_ERR_UNSUPPORTED";
        break;
    case FMOD_ERR_UPDATE:
        result = info ? "An error caused by System::update occured."
                      : "FMOD_ERR_UPDATE";
        break;
    case FMOD_ERR_VERSION:
        result = info ? "The version number of this file format is not supported."
                      : "FMOD_ERR_VERSION";
        break;
    case FMOD_ERR_PRELOADED:
        result = info ? "The specified sound is still in use by the event system, call EventSystem::unloadFSB before trying to release it."
                      : "FMOD_ERR_PRELOADED";
        break;
    case FMOD_ERR_EVENT_FAILED:
        result = info ? "An Event failed to be retrieved, most likely due to 'just fail' being specified as the max playbacks behaviour."
                      : "FMOD_ERR_EVENT_FAILED";
        break;
    case FMOD_ERR_EVENT_INFOONLY:
        result = info ? "Can't execute this command on an EVENT_INFOONLY event."
                      : "FMOD_ERR_EVENT_INFOONLY";
        break;
    case FMOD_ERR_EVENT_INTERNAL:
        result = info ? "An error occured that wasn't supposed to.  See debug log for reason."
                      : "FMOD_ERR_EVENT_INTERNAL";
        break;
    case FMOD_ERR_EVENT_MAXSTREAMS:
        result = info ? "Event failed because 'Max streams' was hit when FMOD_EVENT_INIT_FAIL_ON_MAXSTREAMS was specified."
                      : "FMOD_ERR_EVENT_MAXSTREAMS";
        break;
    case FMOD_ERR_EVENT_MISMATCH:
        result = info ? "FSB mismatches the FEV it was compiled with, the stream/sample mode it was meant to be created with was" " different, or the FEV was built for a different platform."
                      : "FMOD_ERR_EVENT_MISMATCH";
        break;
    case FMOD_ERR_EVENT_NAMECONFLICT:
        result = info ? "A category with the same name already exists."
                      : "FMOD_ERR_EVENT_NAMECONFLICT";
        break;
    case FMOD_ERR_EVENT_NOTFOUND:
        result = info ? "The requested event, event group, event category or event property could not be found."
                      : "FMOD_ERR_EVENT_NOTFOUND";
        break;
    case FMOD_ERR_EVENT_NEEDSSIMPLE:
        result = info ? "Tried to call a function on a complex event that's only supported by simple events."
                      : "FMOD_ERR_EVENT_NEEDSSIMPLE";
        break;
    case FMOD_ERR_EVENT_GUIDCONFLICT:
        result = info ? "An event with the same GUID already exists."
                      : "FMOD_ERR_EVENT_GUIDCONFLICT";
        break;
    case FMOD_ERR_EVENT_ALREADY_LOADED:
        result = info ? "The specified project has already been loaded. Having multiple copies of the same project loaded simult" "aneously is forbidden."
                      : "FMOD_ERR_EVENT_ALREADY_LOADED";
        break;
    case FMOD_ERR_MUSIC_UNINITIALIZED:
        result = info ? "Music system is not initialized probably because no music data is loaded."
                      : "FMOD_ERR_MUSIC_UNINITIALIZED";
        break;
    default:
        if (!IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("false",
                "D:\\Projects\\code\\common\\src.sys\\drv\\sound\\snd_buf_fmod.cpp", 1420, empty_string);
        result = "Unknown error";
        break;
    }
    return result;
}
