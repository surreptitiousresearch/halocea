/* ?Unload@gsINP_KINECT@@QAAXXZ @0x826EEA50 */
#include "../../headers/ws/inp/gsINP_KINECT.h"
#include "../../headers/ws/inp/gsINP_KINECT_boundary.h"
#include "../../headers/ws/ds/dsSTATIC_PARAM_LIST.h"

// 0x826EEA50 — take the Kinect microphone down. Stop speech recognition, register the
// uiSetKinectMicrophoneState UI event exactly once, and broadcast the microphone state as "none".
void gsINP_KINECT::Unload()
{
    if (kinectDevice)
        kinectDevice->DisableSpeechRecognition();

    // One-time lazy event registration (a distinct function-local static from Load's in the
    // original — hence the re-registration guard here).
    static bool s_eventRegistered = false;
    if (!s_eventRegistered) {
        s_eventRegistered = true;
        dsSTRID eventName("uiSetKinectMicrophoneState", false);
        evid_uiSetKinectMicrophoneState = gEventMgr->RegisterEvent(eventName);
    }

    dsSTATIC_PARAM_LIST<1> params;
    params.Add<dsSTRID>("state", SPEECH_ST_NONE);
    gEventMgr->SignalEvent(evid_uiSetKinectMicrophoneState, &params);
    // params destructor releases the inline parameter value.

    _apLog("~Kinect~Kinect unloaded");
    _apLog("~Kinect~uiSetKinectMicrophoneState set to none");
}
