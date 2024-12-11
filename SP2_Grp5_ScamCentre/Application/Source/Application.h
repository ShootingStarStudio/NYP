#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();

	//input
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static double GetTime();
	//int getday();
	static void SetTime(double time);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static unsigned m_width;
	static unsigned m_height;


	static void changescene(int newscene);

	static int dollars;
	static int totalearned;
	static int endtime;
	static int profit;
	static int daydivide;
	
private:

	//Declare a window object
	StopWatch m_timer;
	/*	int time=0;*/
	static int scenenum;
};

#endif