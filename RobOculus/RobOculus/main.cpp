#include "Shared.h"
#include <iostream>
#include <thread>

int main(){
	OculusRift *ovr = new OculusRift;

	std::thread updateOVR(&OculusRift::trackHMD, ovr);
	
	updateOVR.join();

	return 0;
}