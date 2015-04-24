#include "Shared.h"
#include <iostream>
#include <thread>

int main(){
	OculusRift *ovr = new OculusRift;
	XboxController *xbox = new XboxController;

	std::thread updateOVR(&OculusRift::trackHMD, ovr);
	std::thread updateTimer(&XboxController::updateTimer, xbox);//create a thread from a member function
	std::thread updateController(&XboxController::getState, xbox);
	
	updateOVR.join();
	updateTimer.join();
	updateController.join();

	return 0;
}