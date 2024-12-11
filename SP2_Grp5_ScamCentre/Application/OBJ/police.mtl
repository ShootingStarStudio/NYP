#include "ScenePC.h"
#include "SceneGame.h"


ScenePC::ScenePC()
{
}

ScenePC::~ScenePC()
{
}

void ScenePC::Init()
{
	{
		glClearColor(1.f, 1.0f, 1.0f, 0.0f);

		//Enable depth buffer and depth testing
		glEnable(GL_DEPTH_TEST);

		//Enable back face culling
		glEnable(GL_CULL_FACE);

		//Default to fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	{
		//Load vertex and fragment shaders
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		// Get a handle for our "colorTexture" uniform
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		// Get a handle for our "textColor" uniform
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

		// Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		// Use our shader
		glUseProgram(m_programID);
		glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	}
	srand(time(NULL));
	// Init VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{
		meshList[GEO_COIN] = MeshBuilder::GenerateQuad("coin", Color(0, 0, 0), 1.f);
		meshList[GEO_COIN]->textureID = LoadTGA("Image//coin.tga");
		meshList[GEO_COINBOMB] = MeshBuilder::GenerateQuad("coinbomb", Color(0, 0, 0), 1.f);
		meshList[GEO_COINBOMB]->textureID = LoadTGA("Image//coinbomb.tga");
		meshList[GEO_EXPLOSION] = MeshBuilder::GenerateQuad("explosion", Color(0, 0, 0), 1.f);
		meshList[GEO_EXPLOSION]->textureID = LoadTGA("Image//explosion.tga");
		meshList[GEO_WALLPAPER] = MeshBuilder::GenerateQuad("wallpaper", Color(0, 0, 0), 1.f);
		meshList[GEO_WALLPAPER]->textureID = LoadTGA("Image//wallpaper.tga");
		meshList[GEO_CASINOBG] = MeshBuilder::GenerateQuad("cwallpaper", Color(0, 0, 0), 1.f);
		meshList[GEO_CASINOBG]->textureID = LoadTGA("Image//casinoBG.tga");
		meshList[GEO_CASINOBET] = MeshBuilder::GenerateQuad("cwallpaper", Color(0, 0, 0), 1.f);
		meshList[GEO_CASINOBET]->textureID = LoadTGA("Image//casinoBet.tga");

		meshList[GEO_GREENTEXT] = MeshBuilder::GenerateQuad("greentext", 16, 16);
		meshList[GEO_GREENTEXT]->textureID = LoadTGA("Image//greenTextBubble.tga");
		meshList[GEO_BLUETEXT] = MeshBuilder::GenerateQuad("bluetext", 16, 16);
		meshList[GEO_BLUETEXT]->textureID = LoadTGA("Image//bluetext.tga");

		meshList[GEO_LINE] = MeshBuilder::GenerateQuad("line", 16, 16);
		meshList[GEO_LINE]->textureID = LoadTGA("Image//line.tga");

		meshList[GEO_HEADS] = MeshBuilder::GenerateQuad("coinheads", 16, 16);
		meshList[GEO_HEADS]->textureID = LoadTGA("Image//coinhead.tga");
		meshList[GEO_TAILS] = MeshBuilder::GenerateQuad("cointails", 16, 16);
		meshList[GEO_TAILS]->textureID = LoadTGA("Image//cointail.tga"); 
		
		meshList[GEO_HT] = MeshBuilder::GenerateQuad("HT", 16, 16);
		meshList[GEO_HT]->textureID = LoadTGA("Image//headtailsbutton.tga");

		meshList[GEO_EXIT] = MeshBuilder::GenerateQuad("x", Color(0, 0, 0), 1.f);
		meshList[GEO_EXIT]->textureID = LoadTGA("Image//Redx.tga");
	}
	{
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSEPOS] = MeshBuilder::GenerateText("mousepos", 16, 16);
		meshList[GEO_MOUSEPOS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSESTATE] = MeshBuilder::GenerateText("mousestate", 16, 16);
		meshList[GEO_MOUSESTATE]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_SCORE] = MeshBuilder::GenerateText("score", 16, 16);
		meshList[GEO_SCORE]->textureID = LoadTGA("Image//calibri.tga");
	}
	{
		meshList[GEO_METREBARBG] = MeshBuilder::GenerateQuad("metrebarbg", Color(1, 1, 1), 1.f);
		meshList[GEO_METREBARBG]->textureID = LoadTGA("Image//Metrebar.tga");
		meshList[GEO_METREBARFG] = MeshBuilder::GenerateQuad("quad", Color(1, 0.1, 0.1), 1.f);
		meshList[GEO_METREBARBGBG] = MeshBuilder::GenerateQuad("quad", Color(0.4, 0.4, 0.4), 1.f);
	}
	{
		meshList[GEO_CARDBLACK] = MeshBuilder::GenerateQuad("blackcard", Color(1, 1, 1), 1.f);
		meshList[GEO_CARDBLACK]->textureID = LoadTGA("Image//blackCard.tga");
		meshList[GEO_CARDRED] = MeshBuilder::GenerateQuad("redcard", Color(1, 1, 1), 1.f);
		meshList[GEO_CARDRED]->textureID = LoadTGA("Image//redCard.tga");

		meshList[GEO_SUITH] = MeshBuilder::GenerateQuad("heart", 16, 16);
		meshList[GEO_SUITH]->textureID = LoadTGA("Image//heart.tga");
		meshList[GEO_SUITD] = MeshBuilder::GenerateQuad("diamond", 16, 16);
		meshList[GEO_SUITD]->textureID = LoadTGA("Image//diamond.tga");
		meshList[GEO_SUITC] = MeshBuilder::GenerateQuad("Clover", 16, 16);
		meshList[GEO_SUITC]->textureID = LoadTGA("Image//clover.tga");
		meshList[GEO_SUITS] = MeshBuilder::GenerateQuad("Spade", 16, 16);
		meshList[GEO_SUITS]->textureID = LoadTGA("Image//spade.tga");
	}
	{
		meshList[GEO_TUTORIALTEXT] = MeshBuilder::GenerateText("tutorial", 16, 16);
		meshList[GEO_TUTORIALTEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TUTORIALBG] = MeshBuilder::GenerateQuad("tutorialbg", Color(1, 0, 0), 1.f);
		meshList[GEO_QUESTIONMARK] = MeshBuilder::GenerateQuad("tutorialqnmrk", Color(1, 1, 1), 1);
		meshList[GEO_QUESTIONMARK]->textureID = LoadTGA("Image//QuestionMark.tga");
	}

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
}

void ScenePC::Update(double dt)
{
	//mouse inputs
	Application::GetCursorPos(&x, &y);
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	posX = x / w * 80; //convert (0,800) to (0,80)
	posY = 60 - y / h * 60; //convert (600,0) to (0,60)

	static bool bLButtonState = false;
	if (Application::IsKeyPressed('E'))
	{
		dollarsClone = Application::dollars;
	}
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		mousestate = "LBUTTON DOWN";
		if ((posX < 6) && (posY > 54)) {
			tutorial = 1;
		}
		if ((posX > 8 && posX < 15) && (posY > 44.5 && posY < 52))
		{
			gamenum = 1; //coins
			coin1x = rand() % 25 + 45;
			coin1y = rand() % 35 + 15;

		}
		else if ((posX > 17 && posX < 25) && (posY > 44.5 && posY < 52) && boom == false)
		{
			gamenum = 2; //texting
		}
		else if ((posX > 25 && posX < 35) && (posY > 44.5 && posY < 52) && boom == false)
		{
			gamenum = 3; //coin flip
		}
		else if ((posX > 8 && posX < 15) && (posY > 34 && posY < 42) && boom == false)
		{
			gamenum = 4; //blackjack
			//game start declarations
			//bettingvalue = 0;
		}
		else {}
		if (gamenum == 1) {

			if ((posX > coinbombx - 2.5) && (posX < coinbombx + 2.5) && (posY > coinbomby - 2.5) && (posY < coinbomby + 2.5) && boom == false)
			{
				explosionx = 55;
				explosiony = 35;
				Application::dollars -= 500;
				dollarsClone -= 500;
				boom = true;
			}

			if (((posX > coin1x - 2.5) && (posX < coin1x + 2.5) && (posY > coin1y - 2.5) && (posY < coin1y + 2.5) && boom == false) ||
				((posX > coin2x - 2.5) && (posX < coin2x + 2.5) && (posY > coin2y - 2.5) && (posY < coin2y + 2.5) && boom == false))
			{
				coin1x = rand() % 25 + 45;
				coin1y = rand() % 35 + 15;

				coin2x = rand() % 25 + 45;
				coin2y = rand() % 35 + 15;

				while ((coin2x > coin1x - 5) && (coin2x < coin1x + 5) && (coin2y > coin1y - 5) && (coin2y < coin1y + 5))
				{
					coin2x = rand() % 25 + 45;
					coin2y = rand() % 35 + 15;
				}


				coinbombx = rand() % 25 + 45;
				coinbomby = rand() % 35 + 15;
				while ((coinbombx > coin1x - 5) && (coinbombx < coin1x + 5) && (coinbomby > coin1y - 5) && (coinbomby < coin1y + 5) && (coinbombx > coin2x - 5) && (coinbombx < coin2x + 5) && (coinbomby > coin2y - 5) && (coinbomby < coin2y + 5))
				{
					coinbombx = rand() % 25 + 45;
					coinbomby = rand() % 35 + 15;
				}



				dollarsClone += 5;
				Application::dollars += 5;

				coinStarted = true;

			}
		}

		else if (gamenum == 2)
		{
			
			if ((posX > 38 && posX < 56) && (posY > 8 && posY < 21))
			{
				if (correctPos == true) {
					dollarsClone += 4;
					Application::dollars += 4;
				}
				else
				{
					dollarsClone -= 4;
					Application::dollars -= 4;
				}
				RNGmsg = rand() % 8;
				correctPos = rand() % 2;
			}
			if ((posX > 56 && posX < 74) && (posY > 8 && posY < 22))
			{
				if (correctPos == true) {
					dollarsClone += 4;
					Application::dollars += 4;
				}
				else
				{
					dollarsClone -= 4;
					Application::dollars -= 4;
				}
				RNGmsg = rand() % 8;
				correctPos = rand() % 2;
			}

		}
		else if (gamenum == 3)
		{
			if ((posX > 38 && posX < 74) && (posY > 8.5 && posY < 53)) {

				if (rand() % 2 == true)
				{
					cout << "heads" << endl;
					heady = 30;
					taily = 100;
					dollarsClone -= 10;
					Application::dollars -= 10;
				}
				else
				{
					cout << "tails" << endl;
					taily = 30;
					heady = 100;
					dollarsClone += 10;
					Application::dollars += 10;
				}
			}
			gameended = true;
		}
		else if (gamenum == 4)
		{
			if (BJstate == 0) {
				if ((posX > 57 && posX < 74) && (posY > 45 && posY < 52))
				{
					//all in 
					bettingvalue = Application::dollars;
				}
				if ((posX > 52 && posX < 62) && (posY > 18 && posY < 23) && bettingvalue >= 100)
				{
					bettingvalue -= 100;
				}
				if ((posX > 40 && posX < 51) && (posY > 15 && posY < 20) && bettingvalue >= 50)
				{
					bettingvalue -= 50;
				}
				if ((posX > 40 && posX < 51) && (posY > 21 && posY < 26) && bettingvalue >= 10)
				{
					bettingvalue -= 10;
				}
				if ((posX > 40 && posX < 50) && (posY > 35 && posY < 40) && bettingvalue + 10 <= Application::dollars)
				{
					bettingvalue += 10;
				}
				if ((posX > 40 && posX < 50) && (posY > 41 && posY < 46) && bettingvalue + 50 <= Application::dollars)
				{
					bettingvalue += 50;
				}
				if ((posX > 52 && posX < 62) && (posY > 38 && posY < 43) && bettingvalue + 100 <= Application::dollars)
				{
					bettingvalue += 100;
				}
				if ((posX > 61 && posX < 73) && (posY > 27 && posY < 34) && bettingvalue != 0)
				{
					OpenDeck.resetopen();
					OpenDeck.resethand(OpenDeck.dealerhand); OpenDeck.resethand(OpenDeck.playerhand);
					OpenDeck.addcard(OpenDeck.dealerhand);
					OpenDeck.addcard(OpenDeck.playerhand); OpenDeck.addcard(OpenDeck.playerhand);
					Application::dollars -= bettingvalue;
					dollarsClone -= bettingvalue;
					BJstate = 1;
				}
			}
			if (stand == false) {
				if (OpenDeck.valuecount(OpenDeck.dealerhand) <= 21 && OpenDeck.valuecount(OpenDeck.playerhand) <= 21) {
					if ((posX > 38 && posX < 48.5) && (posY > 27 && posY < 34))
					{
						OpenDeck.addcard(OpenDeck.playerhand);  //hit
					}
					else if ((posX > 63 && posX < 74) && (posY > 27 && posY < 34))
					{
						stand = true;  //stand
					}
				}
			}
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		mousestate = "";
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		mousestate = "RBUTTON DOWN";
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		mousestate = "";
	}

	if (BJstate == 1) {
		if (result == 0) {
			if (OpenDeck.valuecount(OpenDeck.playerhand) == 21) {
				result = 3;
			}
			else if (OpenDeck.valuecount(OpenDeck.playerhand) > 21) {
				result = 1;
			}
			else if (stand == true) {
				while (OpenDeck.valuecount(OpenDeck.dealerhand) <= 17) {
					OpenDeck.addcard(OpenDeck.dealerhand);
				}
				if (OpenDeck.valuecount(OpenDeck.dealerhand) > 21) {
					result = 3;
				}
				else if (OpenDeck.valuecount(OpenDeck.dealerhand) == 21) {
					result = 1;
				}
				else if (OpenDeck.valuecount(OpenDeck.playerhand) == OpenDeck.valuecount(OpenDeck.dealerhand)) {
					result = 2;
				}
				else if (OpenDeck.valuecount(OpenDeck.dealerhand) > OpenDeck.valuecount(OpenDeck.playerhand)) {
					result = 1;
				}
				else if (OpenDeck.valuecount(OpenDeck.playerhand) > OpenDeck.valuecount(OpenDeck.dealerhand)) {
					result = 3;
				}
				stand = false;
			}
		}
		else {
			if (result == 1) {
				std::cout << "Player lost" << std::endl;
				//loses bet
			}
			else if (result == 2) {
				std::cout << "Tie/Push" << std::endl;
				//refunds bet
				Application::dollars += bettingvalue;
			}
			else if (result == 3) {
				std::cout << "Player won" << std::endl;
				//gives back bet and extra value
				Application::dollars += bettingvalue * 2;
			}
			result = 0;
			stand = false;
			BJstate = 0;
		}
	}

	int times = Application::GetTime(); // in seconds 
	hours = times % 5;
	day = times / 5;
	if (day == Application::daydivide && times != 0)
	{

		bool dayUp = true;
		Application::daydivide++;
		dollarsClone += Application::profit;
		Application::dollars += Application::profit;
		//std::cout << "if is OK ";

			explosionx = 100;
			explosiony = 100;

			boom = false;
	}

		timeprint = "Day:" + to_string(day) + ",Hour:" + to_string(hours);

		if (coinStarted == true) {
			totalframe++;
			if (totalframe >= 60)
			{
				totalframe = 0;
				seconds--;
			}
			if (seconds < 0) {
				seconds = 0;
				coinStarted = false;
				coin1x = rand() % 25 + 45;
				coin1y = rand() % 35 + 15;


				coin2x = 100;
				coin2y = 100;
				coinbombx = 100;
				coinbomby = 100;
				seconds = 5;
			}
		}
		static bool bEnter = false;
		if (tutorial > 0) {
			if (!bEnter && Application::IsKeyPressed(VK_RETURN)) {
				bEnter = true;
				if (tutorial < 4) {
					tutorial++;
				}
				else
					tutorial = 0;
			}
			if (bEnter && !Application::IsKeyPressed(VK_RETURN)) {
				bEnter = false;
			}
		}
}

void ScenePC::Render()
{
	{
		// Render VBO here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Temp variables
		Mtx44 translate, rotate, scale;
		Mtx44 MVP;

		//These will be replaced by matrix stack soon
		Mtx44 model, view, projection;
		//Set view matrix using camera settings
	}
	//text render
	RenderMeshOnScreen(meshList[GEO_WALLPAPER], 40, 30, 80, 60);
	//UI buttons test
	
	if (gamenum == 1) 
	{
		string coinTimer = "Secs left: " + to_string(seconds);
		RenderTextOnScreen(meshList[GEO_SCORE], coinTimer, Color(0.5, 0.5, 1), 3, 7, 13);
		if (coinStarted == false)
		{
			RenderTextOnScreen(meshList[GEO_SCORE], "click coin to start", Color(0.5, 0.5, 1), 2, 7, 17);
		}
		RenderMeshOnScreen(meshList[GEO_COIN], coin1x, coin1y, 5, 5);
		RenderMeshOnScreen(meshList[GEO_COIN], coin2x, coin2y, 5, 5);
		RenderMeshOnScreen(meshList[GEO_COINBOMB], coinbombx, coinbomby, 5, 5);
	
	}
	else if (gamenum == 2)
	{
		RenderMeshOnScreen(meshList[GEO_GREENTEXT], 55, 35, 2, 2);
		RenderMeshOnScreen(meshList[GEO_BLUETEXT], 46, 15, 1, 1);
		RenderMeshOnScreen(meshList[GEO_BLUETEXT], 66, 15, 1, 1);
		
		
		RenderTextOnScreen(meshList[GEO_SCORE], victimMsg[RNGmsg][0], Color(1, 1, 1), 2, 41, 46);
		RenderTextOnScreen(meshList[GEO_SCORE], victimMsg[RNGmsg][1], Color(1, 1, 1), 2, 41, 43);

		RenderMeshOnScreen(meshList[GEO_LINE], 50, 22, 3, 2);
	

		if (correctPos == 1) {
			RenderTextOnScreen(meshList[GEO_SCORE], correctAns[RNGmsg], Color(0, 0, 0), 2, 38, 16);
			RenderTextOnScreen(meshList[GEO_SCORE], correctAns2[RNGmsg], Color(0, 0, 0), 2, 38, 14);

			RenderTextOnScreen(meshList[GEO_SCORE], wrongAns[RNGmsg], Color(0, 0, 0), 2, 59, 16);
			RenderTextOnScreen(meshList[GEO_SCORE], wrongAns2[RNGmsg], Color(0, 0, 0), 2, 59, 14);
		}
		if (correctPos == 0) {
			RenderTextOnScreen(meshList[GEO_SCORE], wrongAns[RNGmsg], Color(0, 0, 0), 2, 38, 16);
			RenderTextOnScreen(meshList[GEO_SCORE], wrongAns2[RNGmsg], Color(0, 0, 0), 2, 38, 14);

			RenderTextOnScreen(meshList[GEO_SCORE], correctAns[RNGmsg], Color(0, 0, 0), 2, 59, 16);
			RenderTextOnScreen(meshList[GEO_SCORE], correctAns2[RNGmsg], Color(0, 0, 0), 2, 59, 14);
		}
	}
	else if (gamenum == 3)
	{
		RenderTextOnScreen(meshList[GEO_SCORE], "Player will be Tails", Color(0, 0, 0), 2, 42, 16);
		RenderTextOnScreen(meshList[GEO_SCORE], "Bet: 10 dollars", Color(0, 0, 0), 2, 42, 14);
		RenderMeshOnScreen(meshList[GEO_HEADS], 56, heady, 1, 1);
		RenderMeshOnScreen(meshList[GEO_TAILS], 56, taily, 1, 1);
		//RenderMeshOnScreen(meshList[GEO_HT], 56, 25, 2, 2);
	}
	else if (gamenum == 4)
	{
		RenderMeshOnScreen(meshList[GEO_CASINOBG], 40, 30, 80, 60);
		if (BJstate == 0) {
			RenderMeshOnScreen(meshList[GEO_CASINOBET], 40, 30, 80, 60);
			string betvalue = "Bet:" + to_string(bettingvalue);
			RenderTextOnScreen(meshList[GEO_SCORE], betvalue, Color(1, 1, 1), 2.5, 42, 29);

		}
		else if (BJstate == 1) {
			//dealing phase
			//H, D, C, S     3,4,5,6
			//render player hand
			for (size_t i = 0; i < size(OpenDeck.playerhand); i++) {
				if ((OpenDeck.playerhand[i].getsuit() == 3) || (OpenDeck.playerhand[i].getsuit() == 4)) {
					RenderMeshOnScreen(meshList[GEO_CARDRED], cardCoordsX[i], 15, 10, 10);
					string cardnumber = to_string(OpenDeck.playerhand[i].getvalue());
					if (OpenDeck.playerhand[i].getvalue() == 10)
					{
						RenderTextOnScreen(meshList[GEO_SCORE], "1", Color(1, 0, 0), 4, cardCoordsX[i]- 3, 13);
						RenderTextOnScreen(meshList[GEO_SCORE], "0", Color(1, 0, 0), 4, cardCoordsX[i]-1, 13);
					}
					else {
						RenderTextOnScreen(meshList[GEO_SCORE], cardnumber, Color(1, 0, 0), 5, cardCoordsX[i] - 1, 12);						
					}
					if (OpenDeck.playerhand[i].getsuit() == 3) {
						RenderMeshOnScreen(meshList[GEO_SUITH], cardCoordsX[i] - 2, 19, 1, 1); 
					}
					if (OpenDeck.playerhand[i].getsuit() == 4) {
						RenderMeshOnScreen(meshList[GEO_SUITD], cardCoordsX[i] - 2, 19, 1, 1); 
					}
				}
				else if ((OpenDeck.playerhand[i].getsuit() == 5) || (OpenDeck.playerhand[i].getsuit() == 6)) {
					RenderMeshOnScreen(meshList[GEO_CARDBLACK], cardCoordsX[i], 15, 10, 10);
					string cardnumber = to_string(OpenDeck.playerhand[i].getvalue());
					if (OpenDeck.playerhand[i].getvalue() == 10)
					{
						RenderTextOnScreen(meshList[GEO_SCORE], "1", Color(0, 0, 0), 4, cardCoordsX[i] - 3, 13);
						RenderTextOnScreen(meshList[GEO_SCORE], "0", Color(0, 0, 0), 4, cardCoordsX[i] - 1, 13);
					}
					else {
						RenderTextOnScreen(meshList[GEO_SCORE], cardnumber, Color(0, 0, 0), 5, cardCoordsX[i] - 1, 12);
					}
					if (OpenDeck.playerhand[i].getsuit() == 5){
						RenderMeshOnScreen(meshList[GEO_SUITC], cardCoordsX[i] - 2, 19, 1, 1);
					}
					if (OpenDeck.playerhand[i].getsuit() == 6){
						RenderMeshOnScreen(meshList[GEO_SUITS], cardCoordsX[i] - 2, 19, 1, 1);
					}
				}
			}
			//render dealer hand
			for (size_t i = 0; i < size(OpenDeck.dealerhand); i++) {
				if ((OpenDeck.dealerhand[i].getsuit() == 3) || (OpenDeck.dealerhand[i].getsuit() == 4)) {
					RenderMeshOnScreen(meshList[GEO_CARDRED], cardCoordsX[i], 45, 10, 10);
					string cardnumber = to_string(OpenDeck.dealerhand[i].getvalue());
					if (OpenDeck.dealerhand[i].getvalue() == 10)
					{
						RenderTextOnScreen(meshList[GEO_SCORE], "1", Color(1, 0, 0), 4, cardCoordsX[i] - 3, 42);
						RenderTextOnScreen(meshList[GEO_SCORE], "0", Color(1, 0, 0), 4, cardCoordsX[i] - 1, 42);
					}
					else {
						RenderTextOnScreen(meshList[GEO_SCORE], cardnumber, Color(1, 0, 0), 5, cardCoordsX[i] - 1, 42);
					}
					if (OpenDeck.dealerhand[i].getsuit() == 3) {
						RenderMeshOnScreen(meshList[GEO_SUITH], cardCoordsX[i] - 2, 49, 1, 1);
					}
					if (OpenDeck.dealerhand[i].getsuit() == 4) {
						RenderMeshOnScreen(meshList[GEO_SUITD], cardCoordsX[i] - 2, 49, 1, 1);
					}
				}
				else if ((OpenDeck.dealerhand[i].getsuit() == 5) || (OpenDeck.dealerhand[i].getsuit() == 6)) {
					RenderMeshOnScreen(meshList[GEO_CARDBLACK], cardCoordsX[i], 45, 10, 10);
					string cardnumber = to_string(OpenDeck.dealerhand[i].getvalue());
					if (OpenDeck.dealerhand[i].getvalue() == 10)
					{
						RenderTextOnScreen(meshList[GEO_SCORE], "1", Color(0, 0, 0), 4, cardCoordsX[i] - 3, 42);
						RenderTextOnScreen(meshList[GEO_SCORE], "0", Color(0, 0, 0), 4, cardCoordsX[i] - 1, 42);
					}
					else {
						RenderTextOnScreen(meshList[GEO_SCORE], cardnumber, Color(0, 0, 0), 5, cardCoordsX[i] - 1, 42);
					}
					if (OpenDeck.dealerhand[i].getsuit() == 5) {
						RenderMeshOnScreen(meshList[GEO_SUITC], cardCoordsX[i] - 2, 49, 1, 1);
					}
					if (OpenDeck.dealerhand[i].getsuit() == 6) {
						RenderMeshOnScreen(meshList[GEO_SUITS], cardCoordsX[i] - 2, 49, 1, 1);
					}
				}
			}
			string betvalue = "Bet:" + to_string(bettingvalue);
			RenderTextOnScreen(meshList[GEO_SCORE], betvalue, Color(1, 1, 1), 2, 50, 29);
		}
		else {}
	}
	RenderMeshOnScreen(meshList[GEO_QUESTIONMARK], 3, 57, 6, 6);
	if(tutorial > 0){
		RenderTutorial();
	}
	
	string scoreText = "Cash: " + to_string(int(Application::dollars));
	RenderTextOnScreen(meshList[GEO_SCORE], scoreText, Color(0.5, 0.5, 1), 3, 7, 10);
	string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
	RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 2);
	RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 3.5);

	RenderMeshOnScreen(meshList[GEO_EXIT], 78, 58, 4, 4);

	RenderTextOnScreen(meshList[GEO_SCORE], timeprint, Color(0, 0, 0), 2, 7, 19);

	RenderMeshOnScreen(meshList[GEO_EXPLOSION], explosionx, explosiony, 90, 90);
	
	//---------------------------------------------------------
	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
}


void ScenePC::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void ScenePC::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix(); viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix(); modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0); modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * .75f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix(); viewStack.PopMatrix(); modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}


void ScenePC::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void ScenePC::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else {
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ScenePC::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);		//size of screen ui
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
		viewStack.LoadIdentity();		//no need for camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0); modelStack.Scale(sizex, sizey, 0);
	RenderMesh(mesh, false);
		projectionStack.PopMatrix();
		viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void ScenePC::RenderTutorial()
{
	if (tutorial == 1) {
		RenderMeshOnScreen(meshList[GEO_TUTORIALBG], 18, 56, 20, 4);
		RenderTextOnScreen(meshList[GEO_TUTORIALTEXT], "1: Aim Labs", Color(1, 1, 1), 2, 10, 55);
	}
	else if (tutorial == 2) {
		RenderMeshOnScreen(meshList[GEO_TUTORIALBG], 22, 56, 22, 4);
		RenderTextOnScreen(meshList[GEO_TUTORIALTEXT], "2: Text Fraud", Color(1, 1, 1), 2, 13, 55);
	}
	else if (tutorial == 3){
		RenderMeshOnScreen(meshList[GEO_TUTORIALBG], 30, 56, 22, 4);
		RenderTextOnScreen(meshList[GEO_TUTORIALTEXT], "3: Coin Flip", Color(1, 1, 1), 2, 21, 55);
	}
	else if (tutorial == 4) {
		RenderMeshOnScreen(meshList[GEO_TUTORIALBG], 18, 30, 20, 4);
		RenderTextOnScreen(meshList[GEO_TUTORIALTEXT], "4: Black Jack", Color(1, 1, 1), 2, 9, 29);
	}
	RenderTextOnScreen(meshList[GEO_TUTORIALTEXT], "Press Enter to continue", Color(1, 0.5, 0.5), 2, 23, 26);
}
