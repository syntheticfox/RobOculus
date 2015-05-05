#include "Shared.h"
#include <iostream>

#include <windows.h>
#include <Extras\OVR_Math.h>
#include <conio.h>

using namespace std;
using namespace OVR;

#define SERVO_COM "COM6" //serial port for servos arduino

OculusRift::OculusRift(){//initialize the SDK
	ovr_Initialize();

	hmd = ovrHmd_Create(0);

	if (!hmd)
		return;

	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation |
		ovrTrackingCap_Position, ovrTrackingCap_Orientation);

	//init other needed values
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SP = new Serial(SERVO_COM);
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

void OculusRift::resetHMD(){
	ovrHmd_RecenterPose(hmd); //recenter/reset origin
}



void OculusRift::trackHMD(){//eventually can move up yaw and pitch to remove lastYaw and lastPitch
	float lastYaw = 90.0, lastPitch = 90.0;
	float MIN_ACCEL = 2.0;//to determine when to stop moving horizontally

	while (1){
		ovrTrackingState ts = ovrHmd_GetTrackingState(hmd, frameTiming.ScanoutMidpointSeconds);

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)){
			// The cpp compatibility layer is used to convert ovrPosef to Posef (see OVR_Math.h)
			Posef pose = ts.HeadPose.ThePose;
			float yaw, pitch;
			ovrSensorData state = ts.RawSensorData;//use temperature for torque for the continous servo?
			float accel = state.Accelerometer.y;
			
			pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, nullptr);//if using roll, replace nullptr with &roll
			
			// Optional: move cursor back to starting position and print values
			//SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);		

			if (SP->IsConnected()){
				yaw = (RadToDegree(yaw) - 90) * -1;//horizontal axis (left/right)
				if (yaw > 180) yaw -= 360;
				pitch = RadToDegree(pitch) + 90;//vertical axis (up/down)		

				if (accel < MIN_ACCEL){// MAX_ACCEL*-1){//upside-down HMD
					if (accel < 0){
						if (pitch > 90) pitch = lastPitch;
						else pitch = 0;
					}
					else lastPitch = pitch;
					yaw = lastYaw; //stop moving horizontally while at this position
				}
				else lastYaw = yaw; //prob don't always have to do this

				if (yaw < 0 && yaw > -90)yaw = 0;
				else if (yaw < -90)yaw = 180;
				
				char *cstr = new char[2];
				cstr[0] = (char)(yaw / 2);
				cstr[1] = (char)(pitch / 2);
				SP->WriteData(cstr, 2);								
			}
			else{
				//try to connect. won't reconnect for some reason
				//SP->~Serial();
				delete SP;
				SP = new Serial(SERVO_COM);
				Sleep(2500); //will try to connect every 2.5seconds
			}

			if (_kbhit()) {
				//program ends when user hits esc key
				if (SP->IsConnected()){//bring servos back to origin
					char *cstr = new char[2];
					cstr[0] = cstr[1] = (char)(90 / 2);
					SP->WriteData(cstr, 2);
				}
				exit(0);
			}
		}

		Sleep(15);
	}

}

