//this version uses mario kart 8 driving style
#include <iostream>
#include <Windows.h>
#include <mutex>
#include <atomic>
#include <math.h>

#include "Shared.h"
using namespace std;

#define MAX_POWER 127
mutex mtx;
atomic<int> timer;
class MobilePlatform{
private:
	int topLeft_power, botLeft_power, topRight_power, botRight_power;
	bool forward;
	int twist;//0:not twisting, 1: twist right, 2: twist left
	char *serialData;


public:
	MobilePlatform(){ topLeft_power = 0; botLeft_power = 0; topRight_power = 0; botRight_power = 0; forward = true; twist = 0; sp = new Serial("COM4"); serialData = new char[4]; timer = 0; stopped = false; }//COM5 for christian, COM4 for chris
	~MobilePlatform(){
		topLeft_power = 0; botLeft_power = 0; topRight_power = 0; botRight_power = 0;
		sendData();
	}
	Serial *sp;

	bool stopped;
	void setTL_power(int power){ topLeft_power = power; }
	void setTR_power(int power){ topRight_power = power; }
	void setBL_power(int power){ botLeft_power = power; }
	void setBR_power(int power){ botRight_power = power; }
	void setDirection(bool dir){ forward = dir; }
	bool getDirection(){ return forward; }
	void printVals(){
		cout << "\b\b  ";//backspace
		cout << "\r\t\tforw: " << forward << "\tw1: " << topLeft_power << "\tw2: " << topRight_power << "\tw3: " << botLeft_power << "\tw4: " << botRight_power;// << "d: " << ovr->degree;// '/r' prints on the same line

	}
	void sendData();
};

void MobilePlatform::sendData(){
	if (sp->IsConnected()){
		if (twist == 1){
			serialData[0] = 'r';
			serialData[1] = (char)topLeft_power;
		}
		else if (twist == 2){
			serialData[0] = 'l';
			serialData[1] = (char)topLeft_power;
		}
		else if (forward){
			serialData[0] = 'f';
			serialData[1] = (char)topLeft_power;
			serialData[2] = (char)topRight_power;
		}
		else{
			serialData[0] = 'b';
			serialData[1] = (char)botLeft_power;
			serialData[2] = (char)botRight_power;
		}

		if (twist)sp->WriteData(serialData, 2);
		else sp->WriteData(serialData, 3);
	}
}

MobilePlatform *robot = new MobilePlatform;

void XboxController::updateTimer(){
	while (1){
		if (mtx.try_lock()){
			if ((state.Gamepad.wButtons == XINPUT_GAMEPAD_A || state.Gamepad.wButtons == XINPUT_GAMEPAD_B) && timer < 100)timer++;
			else if ((state.Gamepad.wButtons != XINPUT_GAMEPAD_A || state.Gamepad.wButtons != XINPUT_GAMEPAD_B) && timer > 0) timer -= 5;
			else timer = 0;
			mtx.unlock();
		}
		if (!robot->stopped)robot->printVals();
		Sleep(20);
	}

}

XboxController::XboxController(){
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state); //get state of the first controller from XInput
	controllerChange = state.dwPacketNumber;

}

void XboxController::getState(){
	while (1){
		XInputGetState(0, &state); //get state of the first controller from XInput

		if (state.Gamepad.wButtons == XINPUT_GAMEPAD_A || state.Gamepad.wButtons == XINPUT_GAMEPAD_B){
			if (robot->stopped)robot->stopped = false;
			getLS();
			robot->sendData();
		}
		else if ((state.Gamepad.wButtons != XINPUT_GAMEPAD_A || state.Gamepad.wButtons != XINPUT_GAMEPAD_B) && !robot->stopped){//halt wheels when right trigger isn't being pressed
			if (timer == 0)robot->stopped = true;
			int speed = MAX_POWER * timer / 100 / 2; //100 for the timer, 2 to send serially (127 instead of 255)
			//cout << "speed : " << speed << endl;
			robot->setTL_power(speed);
			robot->setTR_power(speed);
			robot->setBL_power(speed);
			robot->setBR_power(speed);
			robot->sendData();

		}
		//else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_Y) 
		else if (controllerChange + 1 < state.dwPacketNumber){
			if (robot->stopped)robot->stopped = false;
			getLS();
			controllerChange = state.dwPacketNumber;
		}
		else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_X){
			robot->setTL_power(0);
			robot->setTR_power(0);
			robot->setBL_power(0);
			robot->setBR_power(0);
			robot->sendData();
		}
		Sleep(150);
	}
}
bool XboxController::isConnected(){
	if (XInputGetState(0, &state) == ERROR_SUCCESS) return true;
	return false;
}
void XboxController::getLS(){
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	float magnitude = sqrt(LX*LX + LY*LY); //how far the controller is pushed

	float normalizedLX = LX / magnitude;//direction the controller was pushed
	float normalizedLY = LY / magnitude;

	float speed = 0;
	if (mtx.try_lock()){
		speed = MAX_POWER * timer / 100 / 2; //half power plus/minus extra speed due to the LS
		mtx.unlock();
	}

	if (state.Gamepad.wButtons == XINPUT_GAMEPAD_A) {//working on here
		robot->setDirection(true);//forward
		robot->setBL_power(speed);
		robot->setBR_power(speed);

		if (normalizedLX < 0){//move to left
			robot->setTL_power(speed + normalizedLX * speed);//RT*normalizedLX);
			robot->setTR_power(speed - normalizedLX * speed);
		}
		else if (normalizedLX > 0){//move to right
			robot->setTL_power(speed + normalizedLX * speed);
			robot->setTR_power(speed - normalizedLX * speed);
		}
		else {//move forward
			robot->setTL_power(speed);
			robot->setTR_power(speed);
		}
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_B){
		robot->setDirection(false);//backwards
		robot->setTL_power(speed);
		robot->setTR_power(speed);

		if (normalizedLX < 0){//move to left
			robot->setBL_power(speed + normalizedLX * speed);
			robot->setBR_power(speed);
		}
		else if (normalizedLX > 0){//move to right
			robot->setBL_power(speed);
			robot->setBR_power(speed - normalizedLX * speed);
		}
		else {//move backwards
			robot->setBL_power(speed);
			robot->setBR_power(speed);
		}
	}

}

