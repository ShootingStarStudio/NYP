#include "Application.h"

//Include GLEW and GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//comment
//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
//scene includes

#include "SceneMainMenu.h"
#include "SceneGame.h"
#include "ScenePC.h"
#include "SceneEnd.h"
//#include "SceneCasinoBJ.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//change this for scene number
int Application::scenenum = 1;

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

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}
Application::~Application()
{
	
}

//mouse input
unsigned Application::m_width;
unsigned Application::m_height;
void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::m_width = w;
	Application::m_height = h;
	glViewport(0, 0, w, h);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
int Application::GetWindowWidth() {
	return m_width;
}
int Application::GetWindowHeight() {
	return m_height;
}

void Application::changescene(int newscene) {
	scenenum = newscene;
}


double Application::GetTime()
{
	return glfwGetTime();
}
void Application::SetTime(double time)
{
    glfwSetTime(time);
}
void Application::Init()
{
	
	//Set the error callback
	glfwSetErrorCallback(error_callback);
	
	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
	m_window = glfwCreateWindow(1920, 1080, "Test Window", NULL, NULL);
	resize_callback(m_window, 1920, 1080);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}
int Application::dollars = 1000;
int Application::totalearned = 0;
int Application::profit = 0;
int Application::endtime = 0;
int Application::daydivide = 1;
void Application::Run()
{  
    Scene* Scenelist[4] = { nullptr, nullptr, nullptr, nullptr};
    Scene* scene1 = new SceneMainMenu(); Scenelist[0] = scene1;
    Scene* scene2; Scene* scene3; Scene* scene4;
    Scenelist[0]->Init();
  
    Scene* scene = Scenelist[0];

    while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
    {
        //Converting Viewport space to UI space
        double x, y;
        Application::GetCursorPos(&x, &y);
        unsigned w = Application::GetWindowWidth();
        unsigned h = Application::GetWindowHeight();
        float posX = (x / w) * 80; //convert (0,800) to (0,80)
        float posY = 60 - (y / h) * 60; //convert (600,0) to (0,60)
        static bool bLButtonState = false;

        if (!bLButtonState && Application::IsMousePressed(0))
        {
            if (scenenum == 1) {

                if ((posX > 5 && posX < 25) && (posY > 20 && posY < 30)) {
                    scenenum = 2;
                    for (size_t i = 1; i < size(Scenelist); i++) {
                        if (Scenelist[i] != nullptr) {
                            Scenelist[i]->Exit();
                            delete Scenelist[i];
                        }
                    }
                    Application::dollars = 1000;
                    Application::totalearned = 0;
                    Application::profit = 0;
                    Application::endtime = 0;
                    Application::daydivide = 1;
                    scene2 = new SceneGame(); Scenelist[1] = scene2;
                    scene3 = new ScenePC(); Scenelist[2] = scene3;
                    scene4 = new SceneEnd(); Scenelist[3] = scene4;
                    Scenelist[1]->Init();
                    Scenelist[2]->Init();
                    Scenelist[3]->Init();
                    Application::SetTime(0);
                }
                if ((posX > 5 && posX < 25) && (posY > 10 && posY < 20)) {
                    exit(0);
                }
            }
            if (scenenum == 3) {
                if ((posX >= 76 && posX <= 80) && (posY >= 56 && posY <= 60)) {
                    scenenum = 2;
                }
            }// hi
            else if (scenenum == 4) {
                if ((posX > 13 && posY < 33) && (posY > 2 && posY < 12)) {
                    scenenum = 1;
                }
                if ((posX > 47 && posX < 67) && (posY > 2 && posY < 12)) {
                    exit(0);
                }
            }
        }
        if (scenenum == 1) {
            scene = Scenelist[0];
        }
        else if (scenenum == 2) {
            scene = Scenelist[1];
        }
        else if (scenenum == 3) {
            scene = Scenelist[2];
        }
        else if (scenenum == 4) {
            scene = Scenelist[3];
        }
        else {}
        scene->Update(m_timer.getElapsedTime());
        scene->Render();


        //Swap buffers
        glfwSwapBuffers(m_window);
        //Get and organize events, like keyboard and mouse input, window resizing, etc...
        glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

    } //Check if the ESC key had been pressed or if the window had been closed

    for (size_t i = 0; i < size(Scenelist); i++) {
        if (Scenelist[i] != nullptr) {
            Scenelist[i]->Exit();
            delete Scenelist[i];
        }
    }
}



void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
