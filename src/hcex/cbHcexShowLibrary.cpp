#include "../headers/hcex/hcex_library_boundary.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ssl/sslERROR.h" // complete type — CallFunc returns sslERROR by value (dtor)
#include <string.h> // strcpy

// cbHcexShowLibrary @ 0x823D6F98 — SSL callback "HcexShowLibrary()". Lazily create the scripted
// "halo_library" entity (running its SSL init), show it, fire its SSL show function, raise the
// script-camera override latch, and — if the model exposes a "camera" node — point ws camera 0 at
// that node's world transform. Source: D:\Projects\code\HCEX\sources\halo_cine.cpp.
extern "C" void cbHcexShowLibrary(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                  sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    (void)retVal;

    if (!hcexLibrary.pHandle || !hcexLibrary.pHandle->pPtr)
    {
        entCREATE_DATA createData; // ctor @ 0x8252FFB8; dtor (~entCREATE_DATA -> ~animCREATE_DATA) at scope exit
        strcpy(createData.name, "halo_library");

        // Force farm "suspend add" across the create so the entity isn't queued for deferred add.
        unsigned char *pSuspendAdd = farmSYSTEM_isSuspendAdd(farmSys);
        unsigned char wasSuspendAdd = *pSuspendAdd;
        *pSuspendAdd = 1;
        entENTITY *library = entCreate(gsScenePtr, "halo_library", nullptr, &createData, nullptr);
        *pSuspendAdd = wasSuspendAdd;

        if (!library)
            return; // createData destroyed by RAII (decompiler inlines ~animCREATE_DATA here)

        library->pInst->state |= 0x4000u;
        // Fire the SSL "library init" function; returned sslERROR discarded.
        library->sslObject.CallFunc(hcexLibraryInit, 0, nullptr, nullptr, sslOBJ_REF(nullptr));
        hcexLibrary.SetPtr(library);
    }

    // Show (this-pointer may be null if the handle was cleared — matches the binary's guarded call).
    entENTITY *shown = hcexLibrary.pHandle ? hcexLibrary.pHandle->pPtr : nullptr;
    shown->Show();

    entENTITY *forShow = hcexLibrary.pHandle ? hcexLibrary.pHandle->pPtr : nullptr;
    forShow->sslObject.CallFunc(hcexLibraryShow, 0, nullptr, nullptr, sslOBJ_REF(nullptr));

    hcex_off_cam = 1;

    entENTITY *forCam = hcexLibrary.pHandle ? hcexLibrary.pHandle->pPtr : nullptr;
    objOBJ *cameraNode = objFindName(forCam->pInst->pObj, "camera");
    if (cameraNode && gsCameraCount() > 0)
    {
        camCAMERA *camera = gsCameraGet(0);
        if (camera)
        {
            // Build the camera basis from the node's local-to-world matrix (matrLT, column-major):
            //   org   = translation (elements 12..14)
            //   up    = elements 4..6, right = elements 0..2, forward = -(elements 8..10)
            m3dV org;
            org.x = cameraNode->matrLT.elements[12];
            org.y = cameraNode->matrLT.elements[13];
            org.z = cameraNode->matrLT.elements[14];
            _m3dCheckValid(&org);

            m3dV forward;
            forward.x = cameraNode->matrLT.elements[8];
            forward.y = cameraNode->matrLT.elements[9];
            forward.z = cameraNode->matrLT.elements[10];
            _m3dCheckValid(&forward);
            _m3dNegateVector(&forward);

            m3dV up;
            up.x = cameraNode->matrLT.elements[4];
            up.y = cameraNode->matrLT.elements[5];
            up.z = cameraNode->matrLT.elements[6];
            _m3dCheckValid(&up);

            m3dV right;
            right.x = cameraNode->matrLT.elements[0];
            right.y = cameraNode->matrLT.elements[1];
            right.z = cameraNode->matrLT.elements[2];
            _m3dCheckValid(&right);

            m3dNormalize(&forward);
            m3dNormalize(&up);
            m3dNormalize(&right);

            // SetParameters(pOrg, pUp, pRight, pForw).
            camera->SetParameters(&org, &up, &right, &forward);
        }
    }
}
