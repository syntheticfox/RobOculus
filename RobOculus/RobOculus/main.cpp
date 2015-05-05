#include "Shared.h"
#include <iostream>
#include <thread>


int main(){
	XboxController *xbox = new XboxController;
	OculusRift *ovr = new OculusRift;
	

	std::thread updateOVR(&OculusRift::trackHMD, ovr);
	std::thread updateTimer(&XboxController::updateTimer, xbox);//create a thread from a member function
	std::thread updateController(&XboxController::getState, xbox, ovr);
	
	updateOVR.join();
	updateTimer.join();
	updateController.join();

	return 0;
}