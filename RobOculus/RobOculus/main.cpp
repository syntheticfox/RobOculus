#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.

<<<<<<< HEAD
//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

//Define an error callback  
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	//glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	//Declare a window object  
	GLFWwindow* window;

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
=======
	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
>>>>>>> origin/openCV
	}

	//unconditional loop
	while (true) {
		Mat cameraFrame;
		stream1.read(cameraFrame);
		imshow("cam", cameraFrame);
		if (waitKey(30) >= 0)
			break;
	}
	return 0;
}