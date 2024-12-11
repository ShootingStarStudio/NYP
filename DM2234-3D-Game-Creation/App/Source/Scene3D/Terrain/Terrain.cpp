/**
CTerrain
@brief Wraps FreeType heightmap loading and rendering, also allowing
	   to use multiple layers of textures with transitions between them.
Adapted from: https://www.mbsoftworks.sk/tutorials/opengl3/21-multilayered-terrain/
Adapted By: Toh Da Jun
Date: Sep 2021
*/
#include "Terrain.h"

#include <vector>

using namespace std;

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ShaderManager
#include "RenderControl/ShaderManager.h"
// Include CTexture
#include "Texture.h"

// Include ImageLoader
#include "System\ImageLoader.h"

/**
 @brief Constructor
 */
CTerrain::CTerrain(void)
	: vRenderScale(glm::vec3(1.0f, 1.0f, 1.0f))
	, vMinPos(glm::vec3(-1.0f))
	, vMaxPos(glm::vec3(1.0f))
{
	// Set the default position of the Terrain
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Destructor
 */
CTerrain::~CTerrain(void)
{
	for (terrain_data map: terrain_vector) {
		ReleaseHeightmap(map);
	}
	terrain_vector.clear();
}

/**
 @brief Initialise this class instance
 @return A bool variable
 */
bool CTerrain::Init(void)
{
	if (LoadAllTextures() == false)
		return false;

	return true;
}

bool CTerrain::Init(const string sImagePath)
{
	// Load all the 5 textures
	if (LoadAllTextures() == false)
		return false;
	if (LoadHeightMapFromImage(sImagePath) == false)
		return false;

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CTerrain::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
 */
bool CTerrain::Update(const double dElapsedTime)
{

	return true;
}

/**
 @brief PreRender.
 */
void CTerrain::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render.
 */
void CTerrain::Render(void)
{
	CShaderManager::GetInstance()->activeShader->setMat4("matrices.projMatrix", projection);
	CShaderManager::GetInstance()->activeShader->setMat4("matrices.viewMatrix", view);

		// We bind all 5 textures - 3 of them are textures for layers, 1 texture is a "path" texture, and last one is
		// the places in heightmap where path should be and how intense should it be
	for (int i = 0; i < 5; i++)
	{
		char sSamplerName[256];
		sprintf(sSamplerName, "gSampler[%d]", i);
		tTextures[i].BindTexture(i);
		CShaderManager::GetInstance()->activeShader->setInt(sSamplerName, i);
	}

	// Create model transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);

	CShaderManager::GetInstance()->activeShader->setMat4("matrices.modelMatrix", model);
	CShaderManager::GetInstance()->activeShader->setMat4("matrices.normalMatrix", glm::mat4(1.0f));
	CShaderManager::GetInstance()->activeShader->setVec4("vColour", glm::vec4(1.0f));

	CShaderManager::GetInstance()->activeShader->setFloat("fRenderHeight", vRenderScale.y);
	CShaderManager::GetInstance()->activeShader->setFloat("fMaxTextureU", float(iCols) * 0.1f);
	CShaderManager::GetInstance()->activeShader->setFloat("fMaxTextureV", float(iRows) * 0.1f);

	CShaderManager::GetInstance()->activeShader->setMat4("HeightmapScaleMatrix", glm::scale(glm::mat4(1.0), glm::vec3(vRenderScale)));

	// Now we're ready to render - we are drawing set of triangle strips using one call, but we g otta enable primitive restart
	glBindVertexArray(terrain_vector[terrain_Index].uiVAO);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(iRows * iCols);

	int iNumIndices = (iRows - 1) * iCols * 2 + iRows - 1;
	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
	glBindVertexArray(0);

	// Unbind the texture and samplers
	for (int i = 0; i < 5; i++)
	{
		tTextures[i].UnBindTexture(i);
	}
}

/**
 @brief PostRender
 */
void CTerrain::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Loads a heightmap and builds up all OpenGL structures for rendering.
 @param sImagePath - path to the (optimally) grayscale
		image containing heightmap data.
 @return A bool variable
*/
bool CTerrain::LoadHeightMapFromImage(const string sImagePath)
{
	// Load the map texture
	int iNrChannels = 0;
	BYTE* bDataPointer = CImageLoader::GetInstance()->Load(sImagePath.c_str(), iCols, iRows, iNrChannels);
	// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (iNrChannels != 3 && iNrChannels != 1))
		return false;

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = iNrChannels;
	// Length of one row in data
	unsigned int row_step = ptr_inc * iCols;

	terrain_data tempstorage;
	//tempstorage.sImagePath = sImagePath;

	tempstorage.vboHeightmapData.CreateVBO();
	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	//vector< vector< glm::vec3> > vVertexData(iRows, vector<glm::vec3>(iCols));
	//vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));

	tempstorage.vVertexData.resize(iRows, vector<glm::vec3>(iCols, glm::vec3(0.0f)));
	tempstorage.vCoordsData.resize(iRows, vector<glm::vec2>(iCols, glm::vec2(0.0f)));

	float fTextureU = float(iCols) * 0.1f;
	float fTextureV = float(iRows) * 0.1f;

	for (int i = 0; i < iRows; i++)
	{
		for (int j = 0; j < iCols; j++)
		{
			float fScaleC = float(j) / float(iCols - 1);
			float fScaleR = float(i) / float(iRows - 1);
			float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			tempstorage.vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			tempstorage.vCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
		}
	}

	// Clear the memory
	delete bDataPointer;
	bDataPointer = NULL;

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];
	for (int i = 0; i < 2; i++)
		vNormals[i] = vector< vector<glm::vec3> >(iRows - 1, vector<glm::vec3>(iCols - 1));

	for (int i = 0; i < iRows - 1; i++)
	{
		for (int j = 0; j < iCols - 1; j++)
		{
			glm::vec3 vTriangle0[] =
			{
				tempstorage.vVertexData[i][j],
				tempstorage.vVertexData[i + 1][j],
				tempstorage.vVertexData[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				tempstorage.vVertexData[i + 1][j + 1],
				tempstorage.vVertexData[i][j + 1],
				tempstorage.vVertexData[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

	vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(iRows, vector<glm::vec3>(iCols));

	for (int i = 0; i < iRows; i++) {
		for (int j = 0; j < iCols; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals
			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i - 1][j - 1];
			// Look for upper-right triangles
			if (i != 0 && j != iCols - 1)
				vFinalNormal += vNormals[0][i - 1][j];
			// Look for bottom-right triangles
			if (i != iRows - 1 && j != iCols - 1)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i][j];
			// Look for bottom-left triangles
			if (i != iRows - 1 && j != 0)
				vFinalNormal += vNormals[1][i][j - 1];
			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
		}
	}

	// First, create a VBO with only vertex data
	int counter = 0;
	tempstorage.vboHeightmapData.CreateVBO(iRows * iCols * (2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	for (int i = 0; i < iRows; i++)
	{
		for (int j = 0; j < iCols; j++)
		{
			tempstorage.vboHeightmapData.AddData(&tempstorage.vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
			tempstorage.vboHeightmapData.AddData(&tempstorage.vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
			tempstorage.vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
			counter++;
		}
	}

	// Now create a VBO with heightmap indices
	/*
		0-1-2
		|/|/|
		3-4-5
		and
		3-4-5
		|/|/|
		6-7-8

		0, 3, 1, 4, 2, 5 for first strip and
		3, 6, 4, 7, 5, 8 for second strip
	*/
	tempstorage.vboHeightmapIndices.CreateVBO();
	int iPrimitiveRestartIndex = iRows * iCols;
	for (int i = 0; i < iRows - 1; i++) {
		for (int j = 0; j < iCols - 1; j++) {
			for (int k = 1; k >= 0; k--) {
				int iRow = i + (1 - k);
				int iIndex = iRow * iCols + j;
				tempstorage.vboHeightmapIndices.AddData(&iIndex, sizeof(int));
			}
		}
		// Restart triangle strips
		tempstorage.vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
	}

	glGenVertexArrays(1, &tempstorage.uiVAO);
	glBindVertexArray(tempstorage.uiVAO);
	// Attach vertex data to this VAO
	tempstorage.vboHeightmapData.BindVBO();
	tempstorage.vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// And now attach index data to this VAO
	// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
	tempstorage.vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	tempstorage.vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
	tempstorage.vboHeightmapData.BindVBO();

	glBindVertexArray(0);

	terrain_vector.push_back(tempstorage);

	return true;
}

/**
 @brief Sets rendering size (scaling) of heightmap.
 @param fRenderX Size in X-axis
 @param fHeight Height in Y-axis
 @param fRenderZ Size in Z-axis
 */
void CTerrain::SetRenderSize(const float fRenderX, const float fHeight, const float fRenderZ)
{
	vRenderScale = glm::vec3(fRenderX, fHeight, fRenderZ);

	// Calculate the min and max positions
	vMinPos = glm::vec3(fRenderX * -0.5f, 0.0f, fRenderZ * -0.5f);
	vMaxPos = glm::vec3(fRenderX * 0.5f, fHeight, fRenderZ * 0.5f);
}

/**
 @brief Sets rendering size (scaling) of heightmap.
 @param fQuadSize How big should be one quad
 @param fHeight - Height of heightmap
 */
void CTerrain::SetRenderSize(const float fQuadSize, const float fHeight)
{
	vRenderScale = glm::vec3(float(iCols) * fQuadSize, fHeight, float(iRows) * fQuadSize);
}

/**
 @brief Releases all data of one heightmap instance.
 */
void CTerrain::ReleaseHeightmap(terrain_data terrain)
{
	terrain.vboHeightmapData.DeleteVBO();
	terrain.vboHeightmapIndices.DeleteVBO();

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glDeleteVertexArrays(1, &terrain.uiVAO);
}

/**
 @brief Get Height Map rows
 @return A int variable
 */
int CTerrain::GetNumHeightmapRows(void) const
{
	return iRows;
}

/**
 @brief Get Height Map columns
 @return A int variable
 */
int CTerrain::GetNumHeightmapCols(void) const
{
	return iCols;
}

/**
 @brief Get the height at a X- and Z-coordinate
 @param fX A const float variable containing the x coordinate to check for the height
 @param fZ A const float variable containing the z coordinate to check for the height
 @return A float variable
 */
float CTerrain::GetHeight(const float fX, const float fZ)
{
	// If it is out of the boundary, then return 0.0f
	//std::cout << vMinPos.x << ", " << vMaxPos.x << std::endl;
	//std::cout << vMinPos.z << ", " << vMaxPos.z << std::endl;
	//read exception
	if ((fX < vMinPos.x || fX >= vMaxPos.x) ||
		(fZ < vMinPos.z || fZ >= vMaxPos.z))
		return 0.0f;

	// Calculate the raw indices for these coordinates
	float fCol = ((fX - (vRenderScale.x * -0.5f)) / vRenderScale.x) * iCols;
	float fRow = ((fZ - (vRenderScale.z * -0.5f)) / vRenderScale.z) * iRows;

	// Calculate the indices for these coordinates
	int iCol = (int)floor(fCol);
	int iRow = (int)floor(fRow);

	// Use Bilinear interpolation of the surrounding 4 indices to smooth out the height
	float fTX = fRow - iRow;
	float fTZ = fCol - iCol;
	float fSampleH1 = terrain_vector[terrain_Index].vVertexData[iRow][iCol].y;
	float fSampleH2 = terrain_vector[terrain_Index].vVertexData[iRow + 1][iCol].y;
	float fSampleH3 = terrain_vector[terrain_Index].vVertexData[iRow][iCol + 1].y;
	float fSampleH4 = terrain_vector[terrain_Index].vVertexData[iRow + 1][iCol + 1].y;
	float fFinalHeight = (fSampleH1 * (1.0f - fTX) + fSampleH2 * fTX) * (1.0f - fTZ) +
		(fSampleH3 * (1.0f - fTX) + fSampleH4 * fTX) * (fTZ);

	return vRenderScale.y * fFinalHeight;
}

/**
 @brief Get the vMinPos
 @return A glm::vec3 variable
 */
glm::vec3 CTerrain::GetMinPos(void) const
{
	return vMinPos;
}

/**
 @brief Get the vMaxPos
 @return A glm::vec3 variable
 */
glm::vec3 CTerrain::GetMaxPos(void) const
{
	return vMaxPos;
}

/**
 @brief Print Self
 */
void CTerrain::PrintSelf(void)
{
	cout << "CTerrain::PrintSelf()" << endl;
	cout << "========================" << endl;
}

void CTerrain::Increment(void) {
	int max = terrain_vector.size();
	terrain_Index++;
	if (terrain_Index >= max) {
		terrain_Index = 0;
	}
}

bool CTerrain::Generate() {					//generate new map and store it in the next index, exports the image with "name, index.ppm"
	double GridX = 10;		//number of cubes on the Xaxis
	double GridY = 10;		//number of cubes on the Yaxis
	double GridZ = 10;		//number of cubes on the Zaxis			//does not actually affect caluclations due to 2d image
	double strength = 1.0;

	unsigned int seed = rand();
	PerlinNoise pn(seed);
	std::vector<std::vector<double>> noiselayers;

	for (unsigned int oct = 0; oct < octave; oct++) {
		std::vector<double> noiseValues(width * height);
		unsigned int kk = 0;
		// Visit every pixel of the image and assign a color generated with Perlin noise
		for (unsigned int i = 0; i < width; ++i) {
			for (unsigned int j = 0; j < height; ++j) {
				double x = (double)i / (double)width;
				double y = (double)j / (double)height; 
				double z = 0;								//height of the 2d image, as a slice of a 3d cube
				double n;									// n is a percentage of 0 to 1, 0 being black and 1 being white

				// Typical Perlin noise
				n = pn.noise(GridX * x, GridY * y, GridZ * z) * strength;

				// Map the values to the [0, 255] interval, use of gray scale for topography
				noiseValues[kk] = floor(255 * n);
				kk++;		//increment in pixel
			}
		}

		//push into image vector
		noiselayers.push_back(noiseValues);

		//update noise info
		GridX *= lacurinity;
		GridY *= lacurinity;
		GridZ *= lacurinity;
		strength *= persistanace;
	}
	//blending all maps
	//ppm fractalimage(width, height);
	std::vector<double> noiseValues(width * height);
	double highest = 0.0, lowest = 255.0;
	for (unsigned int nlayers = 0; nlayers < noiselayers.size(); nlayers++) {
		//opens file from before
		std::vector<double> temp_image = noiselayers.at(nlayers);
		unsigned int kk = 0;
		for (unsigned int i = 0; i < width; ++i) {
			for (unsigned int j = 0; j < height; ++j) {
				noiseValues[kk] += temp_image[kk];
				highest = max(highest, noiseValues[kk]);
				lowest = min(lowest, noiseValues[kk]);
				kk++;		//increment in pixel
			}
		}
	}
	//casting values
	//find the highest and lowest values
	//scale the values from highest to lowest
	//stretch the values to have an even spread of 0-1
	double highestNormalized = 0.0;
	for (int i = 0; i < noiseValues.size(); ++i) {
		noiseValues[i] -= lowest;									//ensure that the smallest value is negated
		noiseValues[i] = pow(noiseValues[i], 2);					//increase height difference
		highestNormalized = max(highestNormalized, noiseValues[i]); //compares to find highest value
	}
	for (int i = 0; i < noiseValues.size(); ++i) {
		noiseValues[i] /= highestNormalized;						//forces values to be between 0 and 1
	}

	//string filename = "PerlinNoise" + to_string(index) + ".ppm";

	//exported the image
	//fractalimage.write(filename);


	//// Load the map texture
	//int iNrChannels = 0;
	//BYTE* bDataPointer = CImageLoader::GetInstance()->Load(sImagePath.c_str(), iCols, iRows, iNrChannels);
	//// We also require our image to be either 24-bit (classic RGB) or 8-bit (luminance)
	//if (bDataPointer == NULL || iRows == 0 || iCols == 0 || (iNrChannels != 3 && iNrChannels != 1))
	//	return false;

	//// How much to increase data pointer to get to next pixel data
	//unsigned int ptr_inc = iNrChannels;
	//// Length of one row in data
	//unsigned int row_step = ptr_inc * iCols;

	terrain_data tempstorage;
	//tempstorage.sImagePath = sImagePath;

	tempstorage.vboHeightmapData.CreateVBO();
	// All vertex data are here (there are iRows*iCols vertices in this heightmap), we will get to normals later
	//vector< vector< glm::vec3> > vVertexData(iRows, vector<glm::vec3>(iCols));
	//vector< vector< glm::vec2> > vCoordsData(iRows, vector<glm::vec2>(iCols));

	tempstorage.vVertexData.resize(height, vector<glm::vec3>(width, glm::vec3(0.0f)));
	tempstorage.vCoordsData.resize(height, vector<glm::vec2>(width, glm::vec2(0.0f)));

	float fTextureU = float(width) * 0.1f;
	float fTextureV = float(height) * 0.1f;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float fScaleC = float(j) / float(width - 1);
			float fScaleR = float(i) / float(height - 1);
			//float fVertexHeight = float(*(bDataPointer + row_step * i + j * ptr_inc)) / 255.0f;
			float fVertexHeight = noiseValues[i * width + j];
			tempstorage.vVertexData[i][j] = glm::vec3(-0.5f + fScaleC, fVertexHeight, -0.5f + fScaleR);
			tempstorage.vCoordsData[i][j] = glm::vec2(fTextureU * fScaleC, fTextureV * fScaleR);
		}
	}

	// Clear the memory
	//delete bDataPointer;
	//bDataPointer = NULL;

	// Normals are here - the heightmap contains ( (iRows-1)*(iCols-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];
	for (int i = 0; i < 2; i++)
		vNormals[i] = vector< vector<glm::vec3> >(height - 1, vector<glm::vec3>(width - 1));

	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			glm::vec3 vTriangle0[] =
			{
				tempstorage.vVertexData[i][j],
				tempstorage.vVertexData[i + 1][j],
				tempstorage.vVertexData[i + 1][j + 1]
			};
			glm::vec3 vTriangle1[] =
			{
				tempstorage.vVertexData[i + 1][j + 1],
				tempstorage.vVertexData[i][j + 1],
				tempstorage.vVertexData[i][j]
			};

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0] - vTriangle0[1], vTriangle0[1] - vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0] - vTriangle1[1], vTriangle1[1] - vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

	vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(height, vector<glm::vec3>(width));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals
			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if (j != 0 && i != 0)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i - 1][j - 1];
			// Look for upper-right triangles
			if (i != 0 && j != width - 1)
				vFinalNormal += vNormals[0][i - 1][j];
			// Look for bottom-right triangles
			if (i != height - 1 && j != width - 1)
				for (int k = 0; k < 2; k++)
					vFinalNormal += vNormals[k][i][j];
			// Look for bottom-left triangles
			if (i != height - 1 && j != 0)
				vFinalNormal += vNormals[1][i][j - 1];
			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
		}
	}

	// First, create a VBO with only vertex data
	int counter = 0;
	tempstorage.vboHeightmapData.CreateVBO(height * width * (2 * sizeof(glm::vec3) + sizeof(glm::vec2))); // Preallocate memory
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tempstorage.vboHeightmapData.AddData(&tempstorage.vVertexData[i][j], sizeof(glm::vec3)); // Add vertex
			tempstorage.vboHeightmapData.AddData(&tempstorage.vCoordsData[i][j], sizeof(glm::vec2)); // Add tex. coord
			tempstorage.vboHeightmapData.AddData(&vFinalNormals[i][j], sizeof(glm::vec3)); // Add normal
			counter++;
		}
	}

	// Now create a VBO with heightmap indices
	/*
		0-1-2
		|/|/|
		3-4-5
		and
		3-4-5
		|/|/|
		6-7-8

		0, 3, 1, 4, 2, 5 for first strip and
		3, 6, 4, 7, 5, 8 for second strip
	*/
	tempstorage.vboHeightmapIndices.CreateVBO();
	int iPrimitiveRestartIndex = height * width;
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			for (int k = 1; k >= 0; k--) {
				int iRow = i + (1 - k);
				int iIndex = iRow * width + j;
				tempstorage.vboHeightmapIndices.AddData(&iIndex, sizeof(int));
			}
		}
		// Restart triangle strips
		tempstorage.vboHeightmapIndices.AddData(&iPrimitiveRestartIndex, sizeof(int));
	}

	glGenVertexArrays(1, &tempstorage.uiVAO);
	glBindVertexArray(tempstorage.uiVAO);
	// Attach vertex data to this VAO
	tempstorage.vboHeightmapData.BindVBO();
	tempstorage.vboHeightmapData.UploadDataToGPU(GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	// And now attach index data to this VAO
	// Here don't forget to bind another type of VBO - the element array buffer, or simplier indices to vertices
	tempstorage.vboHeightmapIndices.BindVBO(GL_ELEMENT_ARRAY_BUFFER);
	tempstorage.vboHeightmapIndices.UploadDataToGPU(GL_STATIC_DRAW);
	tempstorage.vboHeightmapData.BindVBO();

	glBindVertexArray(0);

	terrain_vector.push_back(tempstorage);

	return true;
}
