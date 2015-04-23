#include <OVR_CAPI_0_5_0.h> //possibly just <OVR_CAPI>
#include <Extras\OVR_Math.h>

#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace OVR;
using namespace std;

ovrHmd hmd; //global variables, should be members of class in future
ovrFrameTiming frameTiming;

void Init(){//initialize the SDK
	ovr_Initialize();

	hmd = ovrHmd_Create(0);

	if (!hmd)
		return;

	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
		ovrTrackingCap_Position, ovrTrackingCap_Orientation);
	//ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation | ovrTrackingCap_Position, 0);
}

void Clear(){//a "destructor"
	ovrHmd_Destroy(hmd);

	ovr_Shutdown();
}

void Output(){
	// Optional: we can overwrite the previous console to more
	// easily see changes in values
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(h, &bufferInfo);


	frameTiming = ovrHmd_BeginFrameTiming(hmd, 0);
	while (hmd){

		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
			// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
			Posef pose = ts.HeadPose.ThePose;
			float yaw, pitch, roll;
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

			// Optional: move cursor back to starting position and print values
			SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
			cout << "yaw: " << RadToDegree(yaw) << endl;
			cout << "pitch: " << RadToDegree(pitch) << endl;
			cout << "roll: " << RadToDegree(roll) << endl;

			Sleep(50);



			if (_kbhit()) exit(0);
		}
	}
	ovrHmd_EndFrameTiming(hmd);

}

int main(){
	Init();
	Output();
	Clear();

	return 0;
}