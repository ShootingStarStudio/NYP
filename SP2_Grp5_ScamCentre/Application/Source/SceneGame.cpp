#include "SceneGame.h"
#include <math.h> 

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	{
		// Set background color to dark blue
		glClearColor(0.1f, 0.0f, 0.4f, 0.0f);

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

		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

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
		glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	}

	//Initialize camera settings
	camera.Init(Vector3(0, 120, 160), Vector3(0, 0, 25), Vector3(0, 1, 0));

	// Init VBO
	// camera.position.x, camera.position.y, camera.position.z
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		light[0].position.Set(0, 1000, 0);
		light[0].color.Set(1, 1, 1);
		light[0].power = 0.2f;
		light[0].kC = 1.f;
		light[0].kL = 0.01;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(12.5));
		light[0].cosInner = cos(Math::DegreeToRadian(10));
		light[0].exponent = 1.f;
		light[0].spotDirection.Set(0, 0, 0);

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	}
	for (int i = 0; i < NUM_GEOMETRY; ++i) {
		meshList[i] = nullptr;
	}
	{
		meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(0.25, 0.75, 0.25), 1.f);
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
		meshList[GEO_WORKERAREA] = MeshBuilder::GenerateQuad("quad", Color(0.4, 0.4, 0.4), 1.f);
	}

	{
		meshList[GEO_UPGRADESHOPBG] = MeshBuilder::GenerateQuad("shopbg", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADESHOPFG] = MeshBuilder::GenerateQuad("shopfg", Color(1, 0, 0), 1.f);
		meshList[GEO_LOCKEDFG] = MeshBuilder::GenerateQuad("lockfg", Color(0.5, 0.5, 0.5), 1.f);
		meshList[GEO_UPGRADEITEM1] = MeshBuilder::GenerateQuad("item1", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADEITEM1]->textureID = LoadTGA("Image//Coffee.tga");
		meshList[GEO_UPGRADEITEM2] = MeshBuilder::GenerateQuad("item2", Color(1, 1, 1), 1.f);
		meshList[GEO_UPGRADEITEM2]->textureID = LoadTGA("Image//PoliceCap.tga");
		meshList[GEO_LOCK] = MeshBuilder::GenerateQuad("lock", Color(1, 1, 1), 1.f);
		meshList[GEO_LOCK]->textureID = LoadTGA("Image//Lock.tga");
		meshList[GEO_WORKERUPGRADE] = MeshBuilder::GenerateQuad("WorkerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_WORKERUPGRADE]->textureID = LoadTGA("Image//WorkerUp.tga");
		meshList[GEO_COMPUTERUPGRADE] = MeshBuilder::GenerateQuad("ComputerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_COMPUTERUPGRADE]->textureID = LoadTGA("Image//ComputerUp.tga");
		meshList[GEO_PUPGRADE] = MeshBuilder::GenerateQuad("PermUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_PUPGRADE]->textureID = LoadTGA("Image//PermUpgrade.tga");
		meshList[GEO_WUPGRADE] = MeshBuilder::GenerateQuad("WorkerUpgrade", Color(1, 1, 1), 1.f);
		meshList[GEO_WUPGRADE]->textureID = LoadTGA("Image//ComputerUp.tga");
		meshList[GEO_METREBARBG] = MeshBuilder::GenerateQuad("metrebarbg", Color(1, 1, 1), 1.f);
		meshList[GEO_METREBARBG]->textureID = LoadTGA("Image//Metrebar.tga");
		meshList[GEO_METREBARFG] = MeshBuilder::GenerateQuad("quad", Color(1, 0.1, 0.1), 1.f);
		meshList[GEO_METREBARBGBG] = MeshBuilder::GenerateQuad("quad", Color(0.4, 0.4, 0.4), 1.f);
		meshList[GEO_BRIBE] = MeshBuilder::GenerateQuad("bribe", Color(1, 0.4, 0.4), 1.f);
		meshList[GEO_BRIBE]->textureID = LoadTGA("Image//Bribe.tga");
		meshList[GEO_QUAD_BG] = MeshBuilder::GenerateQuad("shopfg", Color(0, 0, 0), 1.f);
	}

	{
		meshList[GEO_TEA] = MeshBuilder::GenerateOBJ("Tea", "OBJ//cupTea.obj");
		meshList[GEO_PIZZA] = MeshBuilder::GenerateOBJ("Tea", "OBJ//pizzaBox.obj");

		meshList[GEO_PUPGRADEAREA] = MeshBuilder::GenerateQuad("UpgradeArea image", Color(1, 0.4, 0.4), 1.f);
		meshList[GEO_PUPGRADEAREA]->textureID = LoadTGA("Image//PUpgradeArea.tga");
		meshList[GEO_PCAREA] = MeshBuilder::GenerateQuad("PCArea image", Color(1, 0.4, 0.4), 1.f);
		meshList[GEO_PCAREA]->textureID = LoadTGA("Image//PCArea.tga");
	}
	{
		meshList[GEO_ASPHALT] = MeshBuilder::GenerateQuad("road", Color(0.3, 0.3, 0.3), 1.f);
		meshList[GEO_PAINT] = MeshBuilder::GenerateQuad("road paint", Color(1, 1, 1), 1.f);
	}
	{
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSEPOS] = MeshBuilder::GenerateText("mousepos", 16, 16);
		meshList[GEO_MOUSEPOS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_MOUSESTATE] = MeshBuilder::GenerateText("mousestate", 16, 16);
		meshList[GEO_MOUSESTATE]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_COORDS] = MeshBuilder::GenerateText("coordinates", 16, 16);
		meshList[GEO_COORDS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TIME] = MeshBuilder::GenerateText("time", 16, 16);
		meshList[GEO_TIME]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_DOLLARS] = MeshBuilder::GenerateText("dollars", 16, 16);
		meshList[GEO_DOLLARS]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_PROFIT] = MeshBuilder::GenerateText("profit", 16, 16);
		meshList[GEO_PROFIT]->textureID = LoadTGA("Image//calibri.tga");
		meshList[GEO_TUTORIAL] = MeshBuilder::GenerateQuad("tutorial", Color(1, 1, 1), 1);
		meshList[GEO_TUTORIAL]->textureID = LoadTGA("Image//QuestionMark.tga");
	}

	{
		meshList[GEO_SKINNED] = MeshBuilder::GenerateSphere("sphere", Color(1, 0.8, 0.6), 5, 5, 1.f);
		meshList[GEO_SKINNED]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SKINNED]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SKINNED]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SKINNED]->material.kShininess = 1.f;

		meshList[GEO_PANTS] = MeshBuilder::GenerateSphere("sphere", Color(0, 0, 0.4), 5, 5, 1.f);
		meshList[GEO_PANTS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PANTS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PANTS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PANTS]->material.kShininess = 1.f;

		meshList[GEO_SHIRT1] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0.5, 0.5), 5, 5, 1.f);
		meshList[GEO_SHIRT1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT1]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT1]->material.kShininess = 1.f;

		meshList[GEO_SHIRT2] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 1, 0.5), 5, 5, 1.f);
		meshList[GEO_SHIRT2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT2]->material.kShininess = 1.f;

		meshList[GEO_SHIRT3] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0.5, 1), 5, 5, 1.f);
		meshList[GEO_SHIRT3]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_SHIRT3]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SHIRT3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_SHIRT3]->material.kShininess = 1.f;

		shirtrarity[0] = meshList[GEO_SHIRT1]; 
		shirtrarity[1] = meshList[GEO_SHIRT2];
		shirtrarity[2] = meshList[GEO_SHIRT3]; 
	}
	
	{
		meshList[GEO_TABLE] = MeshBuilder::GenerateCube("table", Color(0.54, 0.27, 0.07), 1);
		//meshList[GEO_TABLE]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_TABLE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_TABLE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TABLE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_TABLE]->material.kShininess = 1.f;

		meshList[GEO_ROOM] = MeshBuilder::GenerateQuad("room", Color(0.1, 0.1, 0.5), 1);
		//meshList[GEO_ROOM]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_ROOM]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_ROOM]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_ROOM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_ROOM]->material.kShininess = 1.f;

		meshList[GEO_FLOORTILES] = MeshBuilder::GenerateQuad("roomtiles", Color(1, 1, 1), 1);
		meshList[GEO_FLOORTILES]->textureID = LoadTGA("Image//floor tiles.tga");

		meshList[GEO_OFFICE] = MeshBuilder::GenerateQuad("playeroffice", Color(0.2, 0.2, 0.2), 1);
		//meshList[GEO_OFFICE]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_OFFICE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_OFFICE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_OFFICE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_OFFICE]->material.kShininess = 1.f;

		meshList[GEO_UPGRADEAREA] = MeshBuilder::GenerateQuad("upgradearea", Color(0.1, 0.1, 0.1), 1);
		//meshList[GEO_UPGRADEAREA]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_UPGRADEAREA]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_UPGRADEAREA]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_UPGRADEAREA]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_UPGRADEAREA]->material.kShininess = 1.f;

		meshList[GEO_QUOTE] = MeshBuilder::GenerateQuad("quote", Color(1, 0.4, 0.4), 1.f);
		meshList[GEO_QUOTE]->textureID = LoadTGA("Image//Quotes1.tga");
	}
	{
		meshList[GEO_PHONE] = MeshBuilder::GenerateCube("phone", Color(0.4, 0.4, 0.4), 1);
		meshList[GEO_PHONE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE]->material.kShininess = 1.f;

		meshList[GEO_PHONE1] = MeshBuilder::GenerateCube("phone1", Color(0.7, 0.7, 0.7), 1);
		meshList[GEO_PHONE1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE1]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE1]->material.kShininess = 1.f;

		meshList[GEO_PHONE2] = MeshBuilder::GenerateCube("phone2", Color(0.6, 0.2, 0.2), 1);
		meshList[GEO_PHONE2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PHONE2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PHONE2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PHONE2]->material.kShininess = 1.f;

		meshList[GEO_PAPERSTACK] = MeshBuilder::GenerateCube("paperstack", Color(0.9, 0.9, 0.9), 1);
		//meshList[GEO_PAPERSTACK]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_PAPERSTACK]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PAPERSTACK]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PAPERSTACK]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PAPERSTACK]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP] = MeshBuilder::GenerateCube("laptop", Color(0.6, 0.6, 0.6), 1);
		//meshList[GEO_LAPTOP]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP2] = MeshBuilder::GenerateCube("laptop2", Color(0.05, 0.05, 0.05), 1);
		//meshList[GEO_LAPTOP2]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP2]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP2]->material.kShininess = 1.f;

		meshList[GEO_LAPTOP3] = MeshBuilder::GenerateCube("laptop3", Color(0.3, 0.3, 0.3), 1);
		//meshList[GEO_LAPTOP3]->textureID = LoadTGA("Image//color.tga");
		meshList[GEO_LAPTOP3]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_LAPTOP3]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_LAPTOP3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_LAPTOP3]->material.kShininess = 1.f;
		
		meshList[GEO_INKJAR] = MeshBuilder::GenerateCylinder("inkjar", Color(0.1,0.1, 0.1), 8, 1, 1.f);
		meshList[GEO_INKJAR]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_INKJAR]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_INKJAR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_INKJAR]->material.kShininess = 1.f;

		meshList[GEO_PENBASE] = MeshBuilder::GenerateCylinder("penbase", Color(0.9, 0.9, 0.9), 8, 1, 1.f);
		meshList[GEO_PENBASE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PENBASE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PENBASE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PENBASE]->material.kShininess = 1.f;

		meshList[GEO_FEATHER] = MeshBuilder::GenerateCube("feather", Color(0.7, 0.7, 0.7), 1);
		meshList[GEO_FEATHER]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_FEATHER]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_FEATHER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_FEATHER]->material.kShininess = 1.f;
	}
	
	{
		meshList[GEO_PLANTPOT] = MeshBuilder::GenerateCylinder("plantpot", Color(0.8, 0.4, 0), 20, 1, 1);
		meshList[GEO_PLANTPOT]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PLANTPOT]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PLANTPOT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PLANTPOT]->material.kShininess = 1.f;

		meshList[GEO_PLANTSTEM] = MeshBuilder::GenerateCylinder("plantstem", Color(0.4, 0.2, 0), 20, 1, 1);
		meshList[GEO_PLANTSTEM]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_PLANTSTEM]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_PLANTSTEM]->material.kSpecular.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_PLANTSTEM]->material.kShininess = 1.f;

		meshList[GEO_PLANTLEAF] = MeshBuilder::GenerateCube("plantleaf", Color(0, 0.8, 0), 1);
		meshList[GEO_PLANTLEAF]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_PLANTLEAF]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_PLANTLEAF]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_PLANTLEAF]->material.kShininess = 1.f;

		meshList[GEO_BLUEPRINTS] = MeshBuilder::GenerateCube("blueprints", Color(0, 0.5, 1), 1);
		meshList[GEO_BLUEPRINTS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_BLUEPRINTS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_BLUEPRINTS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_BLUEPRINTS]->material.kShininess = 1.f;

		meshList[GEO_TOOLBOX] = MeshBuilder::GenerateCube("toolbox", Color(0.9, 0, 0), 1);
		meshList[GEO_TOOLBOX]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_TOOLBOX]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TOOLBOX]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_TOOLBOX]->material.kShininess = 1.f;

		meshList[GEO_TOOLBOXHANDLE] = MeshBuilder::GenerateCube("toolboxhandle", Color(0.05, 0.05, 0.05), 1);
		meshList[GEO_TOOLBOXHANDLE]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
		meshList[GEO_TOOLBOXHANDLE]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TOOLBOXHANDLE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_TOOLBOXHANDLE]->material.kShininess = 1.f;
	}
	{
		meshList[GEO_VENDINGMACHINE] = MeshBuilder::GenerateCube("vendingmachine", Color(0, 0, 0), 1);
		meshList[GEO_VENDINGMACHINE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_VENDINGMACHINE]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_VENDINGMACHINE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_VENDINGMACHINE]->material.kShininess = 1.f;

		meshList[GEO_VENDINGMACHINEFRONT] = MeshBuilder::GenerateQuad("vendingmachinefront", Color(0, 0.8, 0), 1);
		meshList[GEO_VENDINGMACHINEFRONT]->textureID = LoadTGA("Image//vendingmachine.tga");
		meshList[GEO_VENDINGMACHINEFRONT]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_VENDINGMACHINEFRONT]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
		meshList[GEO_VENDINGMACHINEFRONT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_VENDINGMACHINEFRONT]->material.kShininess = 1.f;
	}
	
	{
		meshList[GEO_VAN] = MeshBuilder::GenerateOBJMTL("van", "OBJ//van.obj", "OBJ//van.mtl");
		meshList[GEO_VAN]->material.kAmbient.Set(1.f, 1.f, 1.f);
		meshList[GEO_VAN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_VAN]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_VAN]->material.kShininess = 0.f;
		meshList[GEO_SEDAN] = MeshBuilder::GenerateOBJMTL("sedan", "OBJ//sedan.obj", "OBJ//sedan.mtl");
		meshList[GEO_SEDAN]->material.kAmbient.Set(1.f, 1.f, 1.f);
		meshList[GEO_SEDAN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_SEDAN]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_SEDAN]->material.kShininess = 0.f;

		vehicletype[0] = meshList[GEO_VAN];
		vehicletype[1] = meshList[GEO_SEDAN];
	}
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//-------------------------------------------------
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
	debugRot = 20;

	{
		entities[0] = new entity(000, 6, 00); entities[5] = new entity(000, 6, 20);	entities[10] = new entity(000, 6, 40); entities[15] = new entity(000, 6, 60);
		entities[1] = new entity(-15, 6, 00); entities[6] = new entity(-15, 6, 20); entities[11] = new entity(-15, 6, 40); entities[16] = new entity(-15, 6, 60);
		entities[2] = new entity(-30, 6, 00); entities[7] = new entity(-30, 6, 20); entities[12] = new entity(-30, 6, 40); entities[17] = new entity(-30, 6, 60);
		entities[3] = new entity(-45, 6, 00); entities[8] = new entity(-45, 6, 20); entities[13] = new entity(-45, 6, 40); entities[18] = new entity(-45, 6, 60);
		entities[4] = new entity(-60, 6, 00); entities[9] = new entity(-60, 6, 20); entities[14] = new entity(-60, 6, 40); entities[19] = new entity(-60, 6, 60);
	}
}

void SceneGame::Update(double dt)
{

	camera.Update(dt);

	if (Application::GetTime() >= (failstart)) {
		metre.setPoliceFail(false);
	}

	Wages = 0;
	for (int i = 0; i < size(entities); i++) {
		Wages += entities[i]->getWage();
	}

	//player update code
	player.D = -1;
	if (Application::IsKeyPressed('W'))
	{
		player.D = 0;
		player.Z--;
	}
	if (Application::IsKeyPressed('S'))
	{
		player.D = 1;
		player.Z++;
	}
	if (Application::IsKeyPressed('A'))
	{
		player.D = 2;
		player.X--;
	}
	if (Application::IsKeyPressed('D'))
	{
		player.D = 3;
		player.X++;
	}
	if (Application::IsKeyPressed('0'))
	{
		metre.CustomIncreaseMP(100);
	}
	if (Application::IsKeyPressed('9'))
	{
		Application::dollars = -1;
	}
	else {}
	{
		for (int i = 0; i < size(entities); i++) {
			if ((player.X > entities[i]->ECoords[0] - 9) && (player.X < entities[i]->ECoords[0] - 2)) {
				if ((player.Z > entities[i]->ECoords[2] - 5) && (player.Z < entities[i]->ECoords[2] + 5)) {
					if ((player.X > entities[i]->ECoords[0] - 8) && (player.Z > entities[i]->ECoords[2] - 5) && (player.Z < entities[i]->ECoords[2] + 5)) {
						player.X += 1;
					}
					if ((player.X < entities[i]->ECoords[0] - 2.4) && (player.Z > entities[i]->ECoords[2] - 5) && (player.Z < entities[i]->ECoords[2] + 5)) {
						player.X -= 1;
					}
					if ((player.Z > entities[i]->ECoords[2] - 3) && (player.X > entities[i]->ECoords[0] - 10) && (player.X < entities[i]->ECoords[0] - 2)) {
						player.Z += 1;
					}
					if ((player.Z < entities[i]->ECoords[2] + 3) && (player.X > entities[i]->ECoords[0] - 10) && (player.X < entities[i]->ECoords[0] - 2)) {
						player.Z -= 1;
					}
					else {}
				}
			}
		}
		if (player.X > 100) {
			player.X = 100;
		}
		if (player.X < -100) {
			player.X = -100;
		}
		if (player.Z > 75) {
			player.Z = 75;
		}
		if (player.Z < -75) {
			player.Z = -75;
		}
		else {}

		//PC
		if ((player.X > 46 && player.X < 73) && (player.Z > -66 && player.Z < -54)) {
			if ((player.X < 49) && (player.Z > -66 && player.Z < -54)) {
				player.X = 46;
			}
			if ((player.X > 71) && (player.Z > -66 && player.Z < -54)) {
				player.X = 73;
			}
			if ((player.Z < -64) && (player.X > 46 && player.X < 73)) {
				player.Z = -66;
			}
			if ((player.Z > -57) && (player.X > 46 && player.X < 73)) {
				player.Z = -54;
			}
		}

		//Upgrade station
		if ((player.X > 34 && player.X < 60) && (player.Z > 53 && player.Z < 67)) {
			if ((player.X < 36) && (player.Z > 53 && player.Z < 67)) {
				player.X = 34;
			}
			if ((player.X > 58) && (player.Z > 53 && player.Z < 67)) {
				player.X = 60;
			}
			if ((player.Z < 55) && (player.X > 34 && player.X < 60)) {
				player.Z = 53;
			}
			if ((player.Z > 65) && (player.X > 34 && player.X < 60)) {
				player.Z = 67;
			}
		}

		//lounge table
		if ((player.X > -46 && player.X < -33.8) && (player.Z > -49 && player.Z < -33)) {
			if ((player.X < -44) && (player.Z > -49 && player.Z < -33)) {
				player.X = -46;
			}
			if ((player.X > -35) && (player.Z > -49 && player.Z < -33)) {
				player.X = -33.8;
			}
			if ((player.Z < -47) && (player.X > -46 && player.X < -33.8)) {
				player.Z = -49;
			}
			if ((player.Z > -35) && (player.X > -46 && player.X < -33.8)) {
				player.Z = -33;
			}
		}

		//vending machine
		if ((player.X > -56 && player.X < -18) && (player.Z > -100 && player.Z < -65)) {
			if ((player.X < -54) && (player.Z > -100 && player.Z < -65)) {
				player.X = -56;
			}
			if ((player.X > -20) && (player.Z > -100 && player.Z < -65)) {
				player.X = -18;
			}
			if ((player.Z > -67) && (player.X > -56 && player.X < -18)) {
				player.Z = -65;
			}
		}

		if ((player.X > 27 && player.X < 97) && (player.Z > -74 && player.Z < -36))
			changetoPC = true;
		else
		{
			changetoPC = false;
		}
	}

	if (metre.GetMP() > 999) {
		Application::endtime = Application::GetTime();
		SceneEnd::EndingScene(1);
		Application::changescene(4);
	}
	if (Application::dollars < 0) {
		Application::endtime = Application::GetTime();
		SceneEnd::EndingScene(2);
		Application::changescene(4);
	}

	if (Application::dollars >= 2000)
		RenderPermItem1 = true;
	else
		RenderPermItem1 = false;
	if (Application::dollars >= 2500)
		RenderPermItem2 = true;
	else
		RenderPermItem2 = false;
	if (Application::dollars > metre.GetBribeCost())
		Canbribe = true;
	else
		Canbribe = false;
	//mouse inputs
	{
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		posX = x / w * 80; //convert (0,800) to (0,80)
		posY = 60 - y / h * 60; //convert (600,0) to (0,60)
	}
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		mousestate = "LBUTTON DOWN";
		//bribe
		if (Application::dollars > metre.GetBribeCost()) {
			if ((posX > 2.3 && posX < 17.4) && (posY > 46.4 && posY < 53.6))
			{
				failstart = Application::GetTime() + 2;
				Application::dollars -= metre.GetBribeCost();
				metre.Bribe();
			}
		}
		if (PermUpgrade == true) {
			if (RenderPermItem1 == true && coffee == false) {
				if ((posX > 2.4 && posX < 17.4) && (posY > 1.6 && posY < 8.5))
				{
					coffee = true;
					RenderPermItem1 = false;
					mousestate = "Coffee Bought";
					Application::dollars -= 2000;
				}
			}
			if (RenderPermItem2 == true && policedeter == false) {
				if ((posX > 22.4 && posX < 37.4) && (posY > 1.6 && posY < 8.5))
				{
					//Add to police meter later
					policedeter = true;
					RenderPermItem2 = false;
					mousestate = "Police Deterrence Bought";
					Application::dollars -= 2500;
				}
			}
		}
		if (upgrades == true) {
			if ((posX > 2.4 && posX < 17.4) && (posY > 1.6 && posY < 8.5))
			{
				if (entities[entitynumber]->getworkertier() >= 0 && entities[entitynumber]->getworkertier() < 3) {
					if (Application::dollars > entities[entitynumber]->getworkercost()) {
						Application::dollars -= entities[entitynumber]->getworkercost();
						entities[entitynumber]->setworkertier(entities[entitynumber]->getworkertier() + 1);
						std::cout << entitynumber << " worker tier upgrade" << std::endl;
					}
				}
			}
			else if ((posX > 22.4 && posX < 37.4) && (posY > 1.6 && posY < 8.5))
			{
				if (entities[entitynumber]->getstationtier() >= 0 && entities[entitynumber]->getstationtier() < 3) {
					if (Application::dollars > entities[entitynumber]->getstationcost()) {
						Application::dollars -= entities[entitynumber]->getstationcost();
						entities[entitynumber]->setstationtier(entities[entitynumber]->getstationtier() + 1);
						std::cout << entitynumber << " Station tier upgrade" << std::endl;
					}
				}
			}
			else {
				upgrades = false;
			}
		}
		if ((posX > 2 && posX < 9) && (posY > 36 && posY < 44))
		{
			Tutorialmode = 1;
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

	NoobCount = 0; ExperiencedCount = 0; ExpertCount = 0;
	for (int i = 0; i < size(entities); i++) {
		if (entities[i]->getworkertier() == 1) {
			NoobCount++;
		}
		if (entities[i]->getworkertier() == 2) {
			ExperiencedCount++;
		}
		if (entities[i]->getworkertier() == 3) {
			ExpertCount++;
		}
	}

	int times = Application::GetTime(); // in seconds 

	hours = times % 5;
	day = times / 5;
	if (day == Application::daydivide && times != 0)
	{
		dayUp = true;
		Application::daydivide++;

	}
	if (dayUp == true) {
		dayweek++;
		Application::profit = 0;
		metre.DailyIncreaseMP(NoobCount, ExperiencedCount, ExpertCount, policedeter);
		if (dailyprofit > 0) {
			PlaySound(TEXT("money.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		dailyprofit = 0;
		for (int i = 0; i < size(entities); i++) {
			if (coffee == false) {
				/*Application::dollars += entities[i]->getprofit();*/
				/*profit += entities[i]->getprofit();*/
				dailyprofit += entities[i]->getprofit();
				Application::profit = dailyprofit;
				Application::dollars += entities[i]->getprofit();
			}
			else {
				Application::dollars += entities[i]->getprofit() * 1.1;
				Application::profit += entities[i]->getprofit() * 1.1;
				dailyprofit += entities[i]->getprofit() * 1.1;
			}
		}
		Application::totalearned += dailyprofit;
		dayUp = false;
	}
	else {
	}

	if (dayweek == 5) {
		for (int i = 0; i < size(entities); i++) {
			Application::dollars -= entities[i]->getWage();
			dayweek = 0;
		}
	}
	time = "Day:" + to_string(day) + ",Hour:" + to_string(hours);

	if (playerMoving == true)
	{
		if (debugRot <= -40)
		{
			legBack = false;
		}
		if (debugRot >= 40)
		{
			legBack = true;
		}
	}
	else
	{
		debugRot = 0;
	}

	if (legBack == false) {
		debugRot += (float)(40 * dt);
	}
	else
	{
		debugRot += (float)(-40 * dt);
	}

	if (Application::IsKeyPressed('E')) {
		if ((player.X > 27 && player.X < 97) && (player.Z > -74 && player.Z < -36)) {
			Application::changescene(3);
		}
	}
	static bool bEnter = false;
	if(Tutorialmode > 0){
		if (!bEnter && Application::IsKeyPressed(VK_RETURN)) {
			bEnter = true;
			if (Tutorialmode < 4) {
				Tutorialmode++;
			}
			else
				Tutorialmode = 0;
		}
		if (bEnter && !Application::IsKeyPressed(VK_RETURN)) {
			bEnter = false;
		}
	}
}

void SceneGame::Render()
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
			view.SetToLookAt(
				camera.position.x, camera.position.y, camera.position.z,
				camera.target.x, camera.target.y, camera.target.z,
				camera.up.x, camera.up.y, camera.up.z
			);
			viewStack.LoadIdentity();
			viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
				camera.target.x, camera.target.y, camera.target.z,
				camera.up.x, camera.up.y, camera.up.z);
			modelStack.LoadIdentity();
		}
		{
			Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
			/*Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);*/
		}
		//----------------------------------------
		//Render Road
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0.1, 90);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(20, 1000, 1);
			RenderMesh(meshList[GEO_ASPHALT], true);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0); modelStack.Rotate(-90, 1, 0, 0); modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_FLOOR], true);
		modelStack.PopMatrix();
		RenderRoom();
		renderworker(player.X, 5, player.Z, 3);

		if (vehiclex > 200) {
			vehiclex = -200;
			vehiclemodel = rand() % 2;
		}
		vehiclex++;
		modelStack.PushMatrix();
		modelStack.Translate(vehiclex, 0, 90); modelStack.Rotate(90, 0, 1, 0); modelStack.Scale(10, 10, 10);
		RenderMesh(vehicletype[vehiclemodel], true);
		modelStack.PopMatrix();

		for (int i = 0; i < size(entities); i++) {
			RenderTable(entities[i]->ECoords[0] - 5, 3, entities[i]->ECoords[2], entities[i]->getstationtier());
			if (entities[i]->getworkertier() > 0) {
				renderworker(entities[i]->ECoords[0], entities[i]->ECoords[1], entities[i]->ECoords[2], entities[i]->getworkertier());
			}
		}

		if (metre.getPoliceFail() == true) {
			RenderTextOnScreen(meshList[GEO_DOLLARS], "BRIBE FAILED!!!", Color(1, 0, 0), 3, 25, 40);
		}

		////text render
		//string coord = to_string(player.X) + "," + to_string(player.Z);
		//RenderTextOnScreen(meshList[GEO_COORDS], coord, Color(0.5, 0.5, 1), 2, 0, 22.5);
		
		////UI buttons test
		//string mousepos = "posX:" + to_string(posX) + ",posY:" + to_string(posY);
		//RenderTextOnScreen(meshList[GEO_MOUSEPOS], mousepos, Color(0.5, 0.5, 1), 2, 0, 20);
		//RenderTextOnScreen(meshList[GEO_MOUSESTATE], mousestate, Color(0.5, 0.5, 1), 2, 0, 30.5);

		RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(Application::dollars), Color(1,1,1), 2, 2, 57.5);
		RenderMeshOnScreen(meshList[GEO_QUAD_BG], 68, 58, 27, 6);
		RenderTextOnScreen(meshList[GEO_TIME], time, Color(0.5, 1, 0.5), 2, 57, 57.5);
		RenderMeshOnScreen(meshList[GEO_QUAD_BG], 10, 58, 20, 5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(Application::dollars), Color(1, 1, 0), 2, 1, 57.5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "+" + to_string(dailyprofit), Color(1, 1, 0), 2, 1, 55.5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "Weekly Wage:" + to_string(Wages), Color(1, 0, 0), 2, 57, 55.5);

		if(Tutorialmode > 0){
			RenderTutorial(Tutorialmode);
		}
		else if (changetoPC == true) {
				RenderTextOnScreen(meshList[GEO_DOLLARS], "Press E to go to PC", Color(1, 0.5, 0.5), 2, 25, 7);
		}

		RenderMeshOnScreen(meshList[GEO_TUTORIAL], 6, 40, 8, 8);
		if ((player.X < 98 && player.X > 27) && (player.Z < 74 && player.Z > 20)) {
			RenderPermUpgrade();
			PermUpgrade = true;
		}
		else {
			for (int i = 0; i < size(entities); i++) {
				//distance
				float distance = sqrt((player.X - entities[i]->ECoords[0] + 5) * (player.X - entities[i]->ECoords[0] + 5) +
					(player.Z - entities[i]->ECoords[2]) * (player.Z - entities[i]->ECoords[2]));
				if (distance <= 8) {
					entitynumber = i;
					RenderUpgrade();
					upgrades = true;
					break;
				}
				else {
					upgrades = false;
				}
			}
			PermUpgrade = false;
		}

		modelStack.PushMatrix();
		modelStack.Translate(-40, 0, -40);
		RenderLoungeTable();
		modelStack.PopMatrix();
		

		RenderPoliceMetre();
		RenderBribe();
		//---------------------------------------------------------
		Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();
}

void SceneGame::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y) {
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

void SceneGame::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}

	for (int i = 0; i < size(entities); i++) {
		delete entities[i];
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


void SceneGame::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneGame::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

void SceneGame::renderworker(int x, int y, int z, int rarity) {
	rarity--;
	if(rarity >= 0){
		modelStack.PushMatrix();
		modelStack.Translate(x, y, z); modelStack.Scale(1, 1.3, 1);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.Translate(0, -2, 0); modelStack.Scale(1.1, 1.5, 1.1);
		RenderMesh(shirtrarity[rarity], true);

		modelStack.PushMatrix();
		modelStack.Rotate(debugRot, 0, 0, 1);
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -0.3, -1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-debugRot, 0, 0, 1);
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -0.3, 1); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.5, 1, 0.5);
		RenderMesh(shirtrarity[rarity], true);
		modelStack.Translate(0, -1, 0); modelStack.Scale(0.833, 0.666, 0.833);
		RenderMesh(meshList[GEO_SKINNED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(debugRot, 0, 0, 1);
		modelStack.Rotate(-20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Rotate(-debugRot, 0, 0, 1);
		modelStack.Rotate(20, 1, 0, 0);
		modelStack.Translate(0, -1.5, 0); modelStack.Scale(0.833, 0.666, 0.833);
		modelStack.Scale(0.6, 1, 0.6);
		RenderMesh(meshList[GEO_PANTS], true);
		modelStack.PopMatrix();


		modelStack.PopMatrix();
	}
}

void SceneGame::RenderRoom(void)
{
	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Translate(0, 0.01, 0);
		// room floor
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(20, 15, 1);
		RenderMesh(meshList[GEO_FLOORTILES], true);
		modelStack.PopMatrix();

		// room left side
		modelStack.PushMatrix();
		modelStack.Translate(-10, 2.5, 0);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(15, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		// room right side
		modelStack.PushMatrix();
		modelStack.Translate(10, 2.5, 0);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(15, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		// room back side
		modelStack.PushMatrix();
		modelStack.Translate(0, 2.5, -7.5);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(20, 5, 1);
		RenderMesh(meshList[GEO_ROOM], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 1.4, -7.49);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(15, 5, 1);
		RenderMesh(meshList[GEO_QUOTE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(2.2, 0, 7);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(0.2, 0.3, 0.2);
		RenderPlant();
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(0.5, 0, 7);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(0.2, 0.3, 0.2);
		RenderPlant();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(1.35, 0.03, 7);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(1.5, 1, 1);
		RenderMesh(meshList[GEO_UPGRADESHOPFG], true);
		modelStack.PopMatrix();

		// office area
		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.03, -5.5);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(7, 3.5, 1);
		RenderMesh(meshList[GEO_OFFICE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.04, -5.5);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(5, 2, 1);
		RenderMesh(meshList[GEO_PCAREA], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.03, -6);
		modelStack.Scale(0.4, 0.4, 0.4);
		RenderOfficeTable();
		modelStack.PopMatrix();

		// upgrade area
		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.03, 4.75);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(7, 5, 1);
		RenderMesh(meshList[GEO_UPGRADEAREA], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(6.25, 0.04, 4);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(6, 2, 1);
		RenderMesh(meshList[GEO_PUPGRADEAREA], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(4.7, 0.03, 6);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(0.25, 0.25, 0.25);
		RenderUpgradeTable();
		modelStack.PopMatrix();


	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-30, 1, -69);
	modelStack.Scale(8, 8, 8);
	RenderVendingMachine();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-40, 1, -69);
	modelStack.Scale(8, 8, 8);
	RenderVendingMachine();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-50, 1, -69);
	modelStack.Scale(8, 8, 8);
	RenderVendingMachine();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-22, 1, -67);
	modelStack.Scale(3, 3, 3);
	RenderPlant();
	modelStack.PopMatrix();
}

void SceneGame::RenderTable(int x, int y, int z, int tier)
{
	if (tier == 1){
		modelStack.PushMatrix();
		modelStack.Translate(x, y, z + 1);
		modelStack.Rotate(90, 0, 1, 0);
		RenderQuillPaper();
		modelStack.PopMatrix();
	}
	else if (tier == 2) {
		modelStack.PushMatrix();
		modelStack.Translate(x, y, z);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(1.5, 1.5, 1.5);
		RenderPhone();
		modelStack.PopMatrix();
	}
	else if (tier == 3) {
		modelStack.PushMatrix();
		modelStack.Translate(x, y, z);
		modelStack.Rotate(90, 0, 1, 0);
		RenderLaptop();
		modelStack.PopMatrix();
	}

	// tabletop
	modelStack.PushMatrix();
	modelStack.Translate(x, y , z);
	modelStack.Scale(5, 0.2, 5);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.Scale(0.1, 15, 0.1);
	modelStack.Translate(4, -0.5, 4);

	RenderMesh(meshList[GEO_TABLE], true);
	
	modelStack.Translate(0, 0, -8);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.Translate(-8, 0, 0);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.Translate(0, 0, 8);
	RenderMesh(meshList[GEO_TABLE], true);

	modelStack.PushMatrix();
	modelStack.Translate(7, -0.3, -1);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(20, 10, 17);
	RenderMesh(meshList[GEO_UPGRADEAREA], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneGame::RenderQuillPaper()
{
	modelStack.PushMatrix();
	modelStack.Translate(-0.8, 0.25, 0);
	modelStack.Scale(0.25, 0.5, 0.25);
	RenderMesh(meshList[GEO_INKJAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1, 1, 0);
	modelStack.Rotate(20, 0, 0, 1);
	modelStack.Scale(0.01, 1.75, 0.01);
	RenderMesh(meshList[GEO_PENBASE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.15, 1.4, 0);
	modelStack.Rotate(20, 0, 0, 1);
	modelStack.Scale(0.01, 1, 0.25);
	modelStack.Rotate(45, 1, 0, 0);
	RenderMesh(meshList[GEO_FEATHER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, 0);
	modelStack.Scale(2.9, 1, 2.1);
	RenderMesh(meshList[GEO_PAPERSTACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, 0);
	modelStack.Scale(2.9, 1, 2.1);
	RenderMesh(meshList[GEO_PAPERSTACK], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderPhone()
{
	// phone base
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.1, 0);
	modelStack.Scale(2, 0.2, 2);
	RenderMesh(meshList[GEO_PHONE], true);
	// phone screen
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0.4, -0.35);
	modelStack.Scale(0.4, 0.5, 0.2);
	RenderMesh(meshList[GEO_LAPTOP2], true);
	modelStack.PopMatrix();
	// numpad keys
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0.25);
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.35);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.2);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, -0.05);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.1, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE2], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.25, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE1], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0.4, 0, 0.1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_PHONE2], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
	modelStack.PopMatrix();

	//phone
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.4, 0);
	modelStack.Scale(0.75, 0.15, 1.75);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.25, -0.6);
	modelStack.Scale(0.75, 0.4, 0.55);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-0.5, 0.25, 0.6);
	modelStack.Scale(0.75, 0.4, 0.55);
	RenderMesh(meshList[GEO_PHONE], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderLaptop()
{
	// laptop base
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.075, 0);
	modelStack.Rotate(0, 1, 0, 0);
	modelStack.Scale(3.2, 0.15, 2.4);
	RenderMesh(meshList[GEO_LAPTOP], true);
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.4, -0.075);
		modelStack.Scale(0.85, 0.3, 0.4);
		RenderMesh(meshList[GEO_LAPTOP3], true);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(-0.075, 0.4, 0.3);
		modelStack.Scale(0.35, 0.3, 0.3);
		RenderMesh(meshList[GEO_LAPTOP3], true);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// laptop screen
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.1, -1.2);
	modelStack.Rotate(75, 1, 0, 0);
	modelStack.Scale(3.2, 0.15, 2.2);
	RenderMesh(meshList[GEO_LAPTOP], true);
		modelStack.PushMatrix();
		modelStack.Translate(0, 0.5, 0);
		modelStack.Scale(0.85, 0.3, 0.85);
		RenderMesh(meshList[GEO_LAPTOP2], true);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneGame::RenderChair()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	// chair leg 1
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.3, -0.3);
	modelStack.Scale(0.1, 0.6, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
	// chair leg 2
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.3, 0.3);
	modelStack.Scale(0.1, 0.6, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
	// chair leg 3
	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 0.3, -0.3);
	modelStack.Scale(0.1, 0.6, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
	// chair leg 4
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0.3, 0.3);
	modelStack.Scale(0.1, 0.6, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
	// chair seat
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.55, 0);
	modelStack.Scale(0.8, 0.1, 0.8);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
	// chair back
	modelStack.PushMatrix();
	modelStack.Translate(0.3, 1, 0);
	modelStack.Scale(0.07, 0.92, 0.7);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneGame::RenderLoungeTable()
{
	modelStack.PushMatrix();
	modelStack.Scale(2, 3, 5);
	RenderSingleTable();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 6);
	modelStack.Scale(3, 3, 3);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(6, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-6, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2, 5, -2.5);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_TEA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3.5, 5, 2.5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_TEA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 5, -2);
	modelStack.Rotate(-70, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_PIZZA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -7);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	renderworker(0, 6, 0, 1);
	modelStack.PopMatrix();
}

void SceneGame::RenderSingleTable()
{
	// table leg 1
	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, 1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	// table leg 2
	modelStack.PushMatrix();
	modelStack.Translate(2, 0.5, -1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	// table leg 3
	modelStack.PushMatrix();
	modelStack.Translate(-2, 0.5, -1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	// table leg 4
	modelStack.PushMatrix();
	modelStack.Translate(-2, 0.5, 1);
	modelStack.Scale(0.1, 1, 0.1);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	// tabletop
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.1, 0);
	modelStack.Scale(5, 0.2, 2.5);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

}

void SceneGame::RenderPlant()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.75, 0);

	modelStack.PushMatrix();
	modelStack.Scale(0.75, 1.5, 0.75);
	RenderMesh(meshList[GEO_PLANTPOT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.7, 0);
	modelStack.Scale(0.6, 0.2, 0.6);
	RenderMesh(meshList[GEO_PLANTSTEM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.5, 0);
	modelStack.Scale(0.05, 3, 0.05);
	RenderMesh(meshList[GEO_PLANTSTEM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.2, 0);
	modelStack.Rotate(15, 0, 1, 0);
	modelStack.Scale(1.8, 0.05, 1.8);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.56, 0);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(1.6, 0.05, 1.6);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.92, 0);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(1.4, 0.05, 1.4);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2.28, 0);
	modelStack.Rotate(60, 0, 1, 0);
	modelStack.Scale(1.2, 0.05, 1.2);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2.64, 0);
	modelStack.Rotate(75, 0, 1, 0);
	modelStack.Scale(1, 0.05, 1);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 3, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.8, 0.05, 0.8);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();

	modelStack.Translate(0, -0.175, 0);
	modelStack.Rotate(20, 0, 1, 0);

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.2, 0);
	modelStack.Rotate(15, 0, 1, 0);
	modelStack.Scale(1.8, 0.05, 1.8);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.56, 0);
	modelStack.Rotate(30, 0, 1, 0);
	modelStack.Scale(1.6, 0.05, 1.6);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 1.92, 0);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(1.4, 0.05, 1.4);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 2.28, 0);
	modelStack.Rotate(60, 0, 1, 0);
	modelStack.Scale(1.2, 0.05, 1.2);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 2.64, 0);
	modelStack.Rotate(75, 0, 1, 0);
	modelStack.Scale(1, 0.05, 1);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 3, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.8, 0.05, 0.8);
	RenderMesh(meshList[GEO_PLANTLEAF], true);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneGame::RenderToolbox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.5, 0);
	modelStack.Scale(2.5, 1, 1);
	RenderMesh(meshList[GEO_TOOLBOX], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.7, 0);
	modelStack.Scale(2.6, 0.1, 1.1);
	RenderMesh(meshList[GEO_TOOLBOXHANDLE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.7, 0.55);
	modelStack.Scale(0.3, 0.3, 0.1);
	RenderMesh(meshList[GEO_TOOLBOXHANDLE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 1.2, 0);
	modelStack.Scale(0.8, 0.15, 0.2);
	RenderMesh(meshList[GEO_TOOLBOXHANDLE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.3, 1.05, 0);
	modelStack.Scale(0.2, 0.25, 0.2);
	RenderMesh(meshList[GEO_TOOLBOXHANDLE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.3, 1.05, 0);
	modelStack.Scale(0.2, 0.25, 0.2);
	RenderMesh(meshList[GEO_TOOLBOXHANDLE], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderOfficeTable()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	RenderSingleTable();
	modelStack.Translate(0, 1.2, 0.4);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderLaptop();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0.3, 0, 1.3);
	modelStack.Rotate(20, 0, 1, 0);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1.15, 1.2, 0.3);
	modelStack.Rotate(-30, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderPhone();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.15, 1.3, 0.3);
	modelStack.Rotate(-75, 0, 1, 0);
	modelStack.Scale(0.97, 0.33, 0.7);
	RenderMesh(meshList[GEO_PAPERSTACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3.4, 0, 0);
	modelStack.Scale(0.6, 0.6, 0.6);
	RenderPlant();
	modelStack.PopMatrix();
}

void SceneGame::RenderUpgradeTable()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(2, 1, 2);
	RenderSingleTable();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2.5, 1.2, 0);
	modelStack.Rotate(-20, 0, 1, 0);
	modelStack.Scale(3, 0.1, 3.5);
	RenderMesh(meshList[GEO_BLUEPRINTS], true);
	modelStack.Translate(0, 1, 0);
	modelStack.Rotate(20, 0, 1, 0);
	RenderMesh(meshList[GEO_BLUEPRINTS], true);
	modelStack.Translate(0, 1, 0);
	modelStack.Rotate(30, 0, 1, 0);
	RenderMesh(meshList[GEO_BLUEPRINTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2, 1, 0);
	modelStack.Rotate(-20, 0, 1, 0);
	modelStack.Scale(0.8, 0.8, 0.8);
	RenderToolbox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, 0, 2.75);
	modelStack.Rotate(-20, 0, 1, 0);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1.75, 0, 2.85);
	modelStack.Rotate(10, 0, 1, 0);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.75, 0, 2.85);
	modelStack.Rotate(15, 0, 1, 0);
	RenderChair();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.2, 1.2, 1.6);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderLaptop();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2.25, 0, 2.65);
	modelStack.Rotate(-20, 0, 1, 0);
	RenderChair();
	modelStack.PopMatrix();
}

void SceneGame::RenderVendingMachine()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0.79, 0);
	modelStack.Scale(1.22, 1.58, 1);
	RenderMesh(meshList[GEO_VENDINGMACHINE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.79, 0.501);
	modelStack.Scale(1.22, 1.58, 1);
	RenderMesh(meshList[GEO_VENDINGMACHINEFRONT], true);
	modelStack.PopMatrix();
}

void SceneGame::RenderTutorial(int number)
{
	if (number == 1) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 20, 58, 14, 3);
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 20, 57, 14, 3);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "<- Current Balance", Color(1, 1, 1), 1, 13, 58);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "<- Income", Color(1, 1, 1), 1, 13, 56);
	}
	else if (number == 2) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 40, 50, 32, 5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "<- Chance to Decrease", Color(1, 1, 1), 2, 25, 50);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "    Police Gauge", Color(1, 1, 1), 2, 25, 48);
	}
	else if (number == 3) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 54, 35, 24, 5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "<- Worker and", Color(1, 1, 1), 2, 42, 35);
		RenderTextOnScreen(meshList[GEO_DOLLARS], " Station Upgrade", Color(1, 1, 1),	2, 42, 33);
	}
	else if (number == 4) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 56, 35, 23, 5);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "Police Metre ->", Color(1, 1, 1), 2, 45, 35);
		RenderTextOnScreen(meshList[GEO_DOLLARS], "Lose when full", Color(1, 1, 1), 2, 45, 33);
	}
	RenderTextOnScreen(meshList[GEO_DOLLARS], "Press Enter to continue", Color(1, 0.5, 0.5), 2, 25, 7);
}

void SceneGame::RenderPermUpgrade() {
	RenderMeshOnScreen(meshList[GEO_UPGRADESHOPBG], 40, 5, 80, 10);

	if (coffee == false && RenderPermItem1 == true) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_UPGRADEITEM1], 10, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 10, 5, 15, 11);
	}

	if (policedeter == false && RenderPermItem2 == true) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_UPGRADEITEM2], 30, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 30, 5, 15, 11);
	}
	RenderMeshOnScreen(meshList[GEO_PUPGRADE], 60, 5, 20, 7);
}

void SceneGame::RenderUpgrade() {
	RenderMeshOnScreen(meshList[GEO_UPGRADESHOPBG], 40, 5, 80, 10);
	if (Application::dollars > entities[entitynumber]->getworkercost() && entities[entitynumber]->getworkertier() < 3) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_WORKERUPGRADE], 10, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 10, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 10, 5, 15, 11);
	}

	if (Application::dollars > entities[entitynumber]->getstationcost() && entities[entitynumber]->getstationtier() < 3) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_COMPUTERUPGRADE], 30, 5, 7, 7);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 30, 5, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 30, 5, 15, 11);
	}
	
}
//enderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
void SceneGame::RenderPoliceMetre()
{
	RenderMeshOnScreen(meshList[GEO_METREBARBGBG], 73, 33, 5, 21);
	RenderMeshOnScreen(meshList[GEO_METREBARFG], 73, 20, 7, 7);
	RenderMeshOnScreen(meshList[GEO_METREBARFG], 73, 22 + metre.GetMP() * 11 / 1000, 5, metre.GetMP()/ 50); 
	RenderMeshOnScreen(meshList[GEO_METREBARBG], 73, 30, 28, 30);
}

void SceneGame::RenderBribe()
{
	RenderMeshOnScreen(meshList[GEO_QUAD_BG], 17, 50, 15, 7);
	RenderTextOnScreen(meshList[GEO_DOLLARS], "Success:", Color(1, 1, 0), 1, 18, 51);
	RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(metre.getSuccessChance()) + "%", Color(1, 1, 0), 1, 18, 48);
	if(Canbribe == true) {
		RenderMeshOnScreen(meshList[GEO_UPGRADESHOPFG], 10, 50, 15, 7);
		RenderMeshOnScreen(meshList[GEO_BRIBE], 10, 52, 10, 3);
		RenderTextOnScreen(meshList[GEO_DOLLARS], to_string(metre.GetBribeCost()), Color(1, 1, 0), 2, 5, 48);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_LOCKEDFG], 10, 50, 15, 7);
		RenderMeshOnScreen(meshList[GEO_LOCK], 10, 50, 15, 11);
	}
}

