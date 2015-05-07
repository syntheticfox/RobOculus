#include <SerialClass.h>

//GameController
#include <Xinput.h>

//Servos
#include <OVR_CAPI_0_5_0.h>

#define SERVO_COM "COM6" //serial port for servos movement (to arduino)
#define ROBOT_COM "COM5" //serial port for robot movement (to arduino)

class OculusRift{
private: 
	ovrFrameTiming frameTiming;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	Serial* SP = new Serial(SERVO_COM);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

public:
	void trackHMD();
	void resetHMD();
	int degree;
	ovrHmd hmd;
	OculusRift();
	~OculusRift();
};

class XboxController{
private:
	XINPUT_STATE state;
	DWORD controllerChange;
public:
	XboxController();
	~XboxController(){}
	bool isConnected();
	void updateTimer();
	void getState(OculusRift *ovr);
	void getLS();
};
void resetHMD();