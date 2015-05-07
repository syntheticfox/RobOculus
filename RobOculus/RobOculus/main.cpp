#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#define OVR_OS_MAC
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#define OVR_OS_LINUX
#endif


#include "Shared.h"
#include <iostream>
#include <thread>
#include <GL\glew.h>
#include <GLFW\glfw3.h>  
#include <GLFW\glfw3native.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

GLuint createTexture(VideoCapture cap);
Mat rotate(Mat src, double angle);
GLFWmonitor* detect_HMD(OculusRift* ovr);

static void error_callback(int error, const char* description){
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(){
	int width, height;
	union ovrGLConfig config;

	OculusRift *ovr = new OculusRift;
	XboxController *xbox = new XboxController;

	//Initialize GLFW  
	if (!glfwInit())
		std::cout << "GLFW failed to initialize." << std::endl;

	std::thread updateOVRPosition(&OculusRift::trackHMD, ovr);
	std::thread updateTimer(&XboxController::updateTimer, xbox);//create a thread from a member function
	std::thread updateController(&XboxController::getState, xbox, ovr);

	updateOVRPosition.join();
	updateTimer.join();
	updateController.join();
	/*
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	glewExperimental = GL_TRUE;
	if (!glewInit())
		std::cout << "GLEW failed to initialize." << std::endl;

	window = glfwCreateWindow(1020, 960, "RobOculus", detect_HMD(ovr), NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	glfwGetFramebufferSize(window, &width, &height);
	config.OGL.Header.BackBufferSize.w = width;
	config.OGL.Header.BackBufferSize.h = height;
#if defined(_WIN32)
	config.OGL.Window = glfwGetWin32Window(window);
#elif defined(__APPLE__)
#elif defined(__linux__)
	config.OGL.Disp = glfwGetX11Display();
#endif

	VideoCapture capLeft(1);

	
	while (!glfwWindowShouldClose(window)){
		//VideoCapture capLeft(1);
		//ovrPosef headPose[2];
		GLuint textureL = createTexture(capLeft);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//Clear information from last draw
		glClear(GL_COLOR_BUFFER_BIT);
		glDepthMask(GLU_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glNormal3f(0.0, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 0.f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0, 0.f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.f);
		glTexCoord2f(0.0f, 1.0f);
		glEnd();

		glfwSwapBuffers(window);
		//glfwPollEvents(); 
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	*/

	return 0;
}


//------------------------------------------------------------------------------------------------------------------------
GLuint createTexture(VideoCapture cap){
	Mat Frame;

	if (!cap.isOpened()){  		// Check if we succeeded
		cout << "Camera not working." << endl;
		exit(-1);
	}

	GLuint texture;
	cap >> Frame;
	waitKey(0);
	Frame = rotate(Frame, -90);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);	// Create the texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Frame.cols, Frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Frame.data);

	return texture;
}

//-----------------------------------------------------------------------------------------------------------------------
Mat rotate(Mat src, double angle){
	Mat dst;
	cv::Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
	return dst;
}

//------------------------------------------------------------------------------------------------------------------------
//Detect Oculus when in extended mode
GLFWmonitor* detect_HMD(OculusRift* ovr){ // code from official glfw site, detects hmd when in extend mode
	int i, count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	for (i = 0; i < count; i++)
	{
#if defined(_WIN32)
		if (strcmp(glfwGetWin32Monitor(monitors[i]), ovr->hmd->DisplayDeviceName) == 0)
			return monitors[i];
#elif defined(__APPLE__)
		if (glfwGetCocoaMonitor(monitors[i]) == hmd->DisplayId)
			return monitors[i];
#elif defined(__linux__)
		int xpos, ypos;
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &xpos, &ypos);
		if (hmd->WindowsPos.x == xpos &&
			hmd->WindowsPos.y == ypos &&
			hmd->Resolution.w == mode->width &&
			hmd->Resolution.h == mode->height)
		{
			return monitors[i];
		}
#endif
	}
	return monitors[0];
}