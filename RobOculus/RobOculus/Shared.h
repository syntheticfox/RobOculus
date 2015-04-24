#include <SerialClass.h>

//Servos
#include <OVR_CAPI_0_5_0.h>

class OculusRift{
private:
	ovrHmd hmd; //global variables, should be members of class in future
	ovrFrameTiming frameTiming;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	Serial* SP = new Serial("COM5");
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

public:
	void trackHMD();
	int degree;
	OculusRift();
	~OculusRift();
};