#include "../../headers/ws/inp/gsINP_KINECT.h"
#include "../../headers/ws/inp/gsINP_KINECT_boundary.h"
#include "../../headers/ws/ds/dsSTATIC_PARAM_LIST.h"

// 0x826EE930 — bring the Kinect microphone up. Start speech recognition against the current
// grammar, register the uiSetKinectMicrophoneState UI event exactly once, broadcast the
// microphone state as inactive, then switch the device from the main (gameplay) rule set to the
// system rule set.
void gsINP_KINECT::Load()
{
    if (kinectDevice)
        kinectDevice->InitSpeechRecognition(&curGrammarFile);

    // One-time lazy event registration (guarded by a function-local static in the original).
    static bool s_eventRegistered = false;
    if (!s_eventRegistered) {
        s_eventRegistered = true;
        dsSTRID eventName("uiSetKinectMicrophoneState", false);
        evid_uiSetKinectMicrophoneState = gEventMgr->RegisterEvent(eventName);
    }

    dsSTATIC_PARAM_LIST<1> params;
    params.Add<dsSTRID>("state", SPEECH_ST_INACTIVE);
    gEventMgr->SignalEvent(evid_uiSetKinectMicrophoneState, &params);
    // params destructor releases the inline parameter value.

    kinectDevice->DisableSpeechRecognitionRule(&INP_KINECT_MAIN_RULE);
    kinectDevice->EnableSpeechRecognitionRule(&INP_KINECT_SYSTEM_RULE);

    _apLog("~Kinect~Kinect loaded");
    _apLog("~Kinect~uiSetKinectMicrophoneState set to inactive");
}
