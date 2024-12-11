#ifndef SCENE_PC_H
#define SCENE_PC_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Utility.h"

#include "LoadTGA.h"
#include "Utility.h"
#include <iostream>

#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Deck.h"

using namespace std;

class ScenePC : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_FLOOR,
		GEO_QUAD,

		GEO_COIN,
		GEO_COINBOMB,
		GEO_EXPLOSION,

		GEO_WALLPAPER,
		GEO_CASINOBG,
		GEO_CASINOBET,

		GEO_HEADS,
		GEO_TAILS,
		GEO_HT,

		GEO_TUTORIALBG,
		GEO_TUTORIALTEXT,
		GEO_QUESTIONMARK,
	
		//text
		GEO_TEXT,			//debug for text
		GEO_MOUSEPOS,		//debug for mouse
		GEO_MOUSESTATE,		//debug for mouse
		GEO_SCORE,			//number of coins clicked
		GEO_MINING,			//Instructions
		
		GEO_LINE,
		GEO_EXIT,
		GEO_GREENTEXT,
		GEO_BLUETEXT,

		GEO_METREBARBG,
		GEO_METREBARFG,
		GEO_METREBARBGBG,

		//cards
		GEO_CARDBLACK,
		GEO_CARDRED,
		GEO_SUITH,
		GEO_SUITD,
		GEO_SUITS,
		GEO_SUITC,
		//card text
		GEO_REDTEXT,
		GEO_BLACKTEXT,		//also for betting phase

		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

public:
	ScenePC();
	~ScenePC();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderSmallTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTutorial();

	//mouse input
	double x = 0, y = 0;
	float posX = 0, posY = 0;
	string mousestate;

	string timeprint;
	int day = 0;
	int hours=0;


	int gamenum = 2;

	int coin1x = rand() % 25 + 45;
	int coin1y = rand() % 50 + 10;

	int coin2x = 100;
	int coin2y = 100;

	int coinbombx = 100;
	int coinbomby = 100;
	
	int explosionx = 100;
	int explosiony = 100;

	bool boom = false;

	int score = 0;
	int minescore = 0;
	int textscore = 0;

	int heady = 30;
	int taily = 100;
	float CFscale = 1;

	int dollarsClone=0;

	int seconds = 5; //coin game timer
	bool coinStarted = false;

	int daydivide = 1;

	int totalframe = 0;

	bool gameended = false;

	std::string victimMsg[8][2] = 
	{
		{"What company are", "you calling from?"},
		{"Are you a scammer?", "  "},
		{"How do I stop the" ,"police from coming???"},
		{"How do I receive ","the product refund?"},
		{"How 2 receive this ","lottery prize? lol"},
		{"how to get FREE"," discerds nitre"},
		{"How do I stop my acc","from being suspended"},
		{"Give me back my son!", "  "}
	};

	std::string correctAns[8]
	{
		"Amazon",
		"No",
		"Give credit",
		"Give credit",
		"give a",
		"Give account",
		"pay up",
		"hand over",
	};

	std::string correctAns2[8]
	{
		"  ",
		"  ",
		"card no.",
		"card no.",
		"deposit",
		"details",
		"  ",
		"the money",
	};

	std::string wrongAns[8]
	{
		"amazoone",
		"Yes",
		"you can't",
		"go to our ",
		"It was a",
		"buy it ",
		"Nothing",
		"no",
	};

	std::string wrongAns2[8]
	{
		"  ",
		"  ",
		"  ",
		"office",
		"prank",
		"yourself",
		"  ",
		"  ",
	};

	int RNGmsg = rand() % 8;
	bool correctPos = rand() % 2;

	int BJstate = 0;
	int bettingvalue = 0;
	int result = 0;
	Deck OpenDeck;
	bool stand = false;

	int cardCoordsX[5] = {42,46,50,54,58};

	int tutorial = 0;

	string BJmsg = " ";

};



#endif