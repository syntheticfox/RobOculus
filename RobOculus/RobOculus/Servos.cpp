#include "Shared.h"
#include <iostream>

#include <windows.h>
#include <Extras\OVR_Math.h>
#include <conio.h>

using namespace std;
using namespace OVR;

OculusRift::OculusRift(){//initialize the SDK
	ovr_Initialize();

	hmd = ovrHmd_Create(0);

	if (!hmd)
		return;

	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
		ovrTrackingCap_Position, ovrTrackingCap_Orientation);


	//init other needed values
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SP = new Serial("COM5");
	//begin head tracking
	// Optional: we can overwrite the previous console to more
	// easily see changes in values
	GetConsoleScreenBufferInfo(h, &bufferInfo);//not sure if this is needed
	frameTiming = ovrHmd_BeginFrameTiming(hmd, 0);

	ovrHmd_RecenterPose(hmd); //recenter/reset origin
}

OculusRift::~OculusRift(){//a "destructor"
	ovrHmd_EndFrameTiming(hmd);//stop head tracking
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();
}

void OculusRift::trackHMD(){
	//cout << "got in" << endl;
	while (1){
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)){
			// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
			Posef pose = ts.HeadPose.ThePose;
			float yaw, pitch, roll;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

			// Optional: move cursor back to starting position and print values
			//SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);

			//if (SP->IsConnected()){
			degree = (RadToDegree(yaw) - 90) * -1;
			char *cstr = new char[2];

			if (degree < 0)degree = 0;
			else if (degree > 180)degree = 180;

			cstr[0] = (char)(degree / 2);
			//cout << "\rDegree: " << degree << endl;
			//SP->WriteData(cstr, 1);

			//}
			if (_kbhit()) exit(0);
		}

		Sleep(150);
	}

}

