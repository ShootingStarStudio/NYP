/**
 Map2D
 @brief A class which manages the map in the game
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Map2D.h"

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include Filesystem
#include "System\filesystem.h"
// Include ImageLoader
#include "System\ImageLoader.h"
#include "Primitives/MeshBuilder.h"

#include <iostream>
#include <vector>
using namespace std;
using namespace std::placeholders;
/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CMap2D::CMap2D(void)
	: uiCurLevel(0)
	, quadMesh(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CMap2D::~CMap2D(void)
{
	// Dynamically deallocate the 3D array used to store the map information
	for (unsigned int uiLevel = 0; uiLevel < uiNumLevels; uiLevel++)
	{
		for (unsigned int iRow = 0; iRow < cSettings->NUM_TILES_YAXIS; iRow++)
		{
			delete[] arrMapInfo[uiLevel][iRow];
		}
		delete [] arrMapInfo[uiLevel];
	}
	delete[] arrMapInfo;

	if (quadMesh)
	{
		delete quadMesh;
		quadMesh = NULL;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Set this to NULL since it was created elsewhere, so we let it be deleted there.
	cSettings = NULL;

	// Delete AStar lists
	DeleteAStarLists();
}

/**
@brief Init Initialise this instance
*/ 
bool CMap2D::Init(	const unsigned int uiNumLevels,
					const unsigned int uiNumRows,
					const unsigned int uiNumCols)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Create the arrMapInfo and initialise to 0
	// Start by initialising the number of levels
	arrMapInfo = new Grid** [uiNumLevels];
	for (unsigned uiLevel = 0; uiLevel < uiNumLevels; uiLevel++)
	{
		arrMapInfo[uiLevel] = new Grid* [uiNumRows];
		for (unsigned uiRow = 0; uiRow < uiNumRows; uiRow++)
		{
			arrMapInfo[uiLevel][uiRow] = new Grid[uiNumCols];
			for (unsigned uiCol = 0; uiCol < uiNumCols; uiCol++)
			{
				arrMapInfo[uiLevel][uiRow][uiCol].value = 0;
			}
		}
	}

	// Store the map sizes in cSettings
	uiCurLevel = 0;
	this->uiNumLevels = uiNumLevels;
	cSettings->NUM_TILES_XAXIS = uiNumCols;
	cSettings->NUM_TILES_YAXIS = uiNumRows;
	cSettings->UpdateSpecifications();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_GroundTile.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_GroundTile.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(100, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_BrickTile.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_BrickTile.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(101, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Campfire.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Campfire.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(99, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_SwampTile.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_SwampTile.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(98, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Mushroom.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Mushroom.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(110, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_TreeTrunk.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_TreeTrunk.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(111, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Leaves.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Mushroom.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(112, iTextureID));

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Herb.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Herb.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(2, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Bamboo.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Bamboo.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(20, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Vine.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Vine.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(97, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Scaffold.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Scaffold.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(96, iTextureID));

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Bow.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Bow.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(3, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Quiver.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Quiver.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(4, iTextureID));
	
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Shrub.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Shrub.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(114, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Stick.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Stick.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(6, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_MangoLeaves.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_MangoLeaves.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(113, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Mango.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Mango.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(5, iTextureID));

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_ArrowLeft.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_ArrowLeft.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(10, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_ArrowRight.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_ArrowRight.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(11, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Target.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Target.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(115, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_LeverOff.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_LeverOff.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(12, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_LeverOn.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_LeverOn.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(13, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_DoorClosed.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_DoorClosed.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(116, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_DoorOpen.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_DoorOpen.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(14, iTextureID));

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Key.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Key.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(15, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Chest.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_Chest.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(16, iTextureID));
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_ChestOpen.tga", true);
	if (iTextureID == 0) {
		cout << "Unable to load Image/Scene2D_ChestOpen.tga" << endl;
		return false;
	}
	else MapOfTextureIDs.insert(pair<int, int>(17, iTextureID));

	// Initialise the variables for AStar
	m_weight = 1;
	m_startPos = glm::vec2(0, 0);
	m_targetPos = glm::vec2(0, 0);
	//m_size = cSettings->NUM_TILES_YAXIS* cSettings->NUM_TILES_XAXIS;
	m_nrOfDirections = 4;
	m_directions = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 },
						{ -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 } };
	// Resize these 2 lists
	m_cameFromList.resize(cSettings->NUM_TILES_YAXIS * cSettings->NUM_TILES_XAXIS);
	m_closedList.resize(cSettings->NUM_TILES_YAXIS * cSettings->NUM_TILES_XAXIS, false);

	return true;
}

/**
@brief Update Update this instance
*/
void CMap2D::Update(const double dElapsedTime)
{
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CMap2D::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 */
void CMap2D::Render(void)
{
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Render
	for (unsigned int uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
	{
		for (unsigned int uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; uiCol++)
		{
			transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transform = glm::translate(transform, glm::vec3(cSettings->ConvertIndexToUVSpace(cSettings->x, uiCol, false, 0),
															cSettings->ConvertIndexToUVSpace(cSettings->y, uiRow, true, 0),
															0.0f));
			//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			// Update the shaders with the latest transform
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			// Render a tile
			RenderTile(uiRow, uiCol);
		}
	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CMap2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

// Set the specifications of the map
void CMap2D::SetNumTiles(const CSettings::AXIS sAxis, const unsigned int uiValue)
{
	// Check if the value is valid
	if (uiValue <= 0)
	{
		cout << "CMap2D::SetNumTiles() : value must be more than 0" << endl;
		return;
	}

	if (sAxis == CSettings::x)
	{
		cSettings->NUM_TILES_XAXIS = uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::y)
	{
		cSettings->NUM_TILES_YAXIS = uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::z)
	{
		// Not used in here
	}
	else
	{
		cout << "Unknown axis" << endl;
	}
}

// Set the specifications of the map
void CMap2D::SetNumSteps(const CSettings::AXIS sAxis, const unsigned int uiValue)
{
	// Check if the value is valid
	if (uiValue <= 0)
	{
		cout << "CMap2D::SetNumSteps() : value must be more than 0" << endl;
		return;
	}

	if (sAxis == CSettings::x)
	{
		cSettings->NUM_STEPS_PER_TILE_XAXIS = (float)uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::y)
	{
		cSettings->NUM_STEPS_PER_TILE_YAXIS = (float)uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::z)
	{
		// Not used in here
	}
	else
	{
		cout << "Unknown axis" << endl;
	}
}

/**
 @brief Set the value at certain indices in the arrMapInfo
 @param iRow A const int variable containing the row index of the element to set to
 @param iCol A const int variable containing the column index of the element to set to
 @param iValue A const int variable containing the value to assign to this arrMapInfo
 */
void CMap2D::SetMapInfo(const unsigned int uiRow, const unsigned int uiCol, const int iValue, const bool bInvert)
{
	if (bInvert)
		arrMapInfo[uiCurLevel][cSettings->NUM_TILES_YAXIS - uiRow - 1][uiCol].value = iValue;
	else
		arrMapInfo[uiCurLevel][uiRow][uiCol].value = iValue;
}

/**
 @brief Get the value at certain indices in the arrMapInfo
 @param iRow A const int variable containing the row index of the element to get from
 @param iCol A const int variable containing the column index of the element to get from
 @param bInvert A const bool variable which indicates if the row information is inverted
 */
int CMap2D::GetMapInfo(const unsigned int uiRow, const int unsigned uiCol, const bool bInvert) const
{
	if (bInvert)
		return arrMapInfo[uiCurLevel][cSettings->NUM_TILES_YAXIS - uiRow - 1][uiCol].value;
	else
		return arrMapInfo[uiCurLevel][uiRow][uiCol].value;
}

/**
 @brief Load a map
 */ 
bool CMap2D::LoadMap(string filename, const unsigned int uiCurLevel)
{
	doc = rapidcsv::Document(FileSystem::getPath(filename).c_str());

	// Check if the sizes of CSV data matches the declared arrMapInfo sizes
	if ((cSettings->NUM_TILES_XAXIS != (unsigned int)doc.GetColumnCount()) ||
		(cSettings->NUM_TILES_YAXIS != (unsigned int)doc.GetRowCount()))
	{
		cout << "Sizes of CSV map does not match declared arrMapInfo sizes." << endl;
		return false;
	}

	// Read the rows and columns of CSV data into arrMapInfo
	for (unsigned int uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
	{
		// Read a row from the CSV file
		std::vector<std::string> row = doc.GetRow<std::string>(uiRow);
		
		// Load a particular CSV value into the arrMapInfo
		for (unsigned int uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; ++uiCol)
		{
			arrMapInfo[uiCurLevel][uiRow][uiCol].value = (int)stoi(row[uiCol]);
		}
	}

	return true;
}

/**
 @brief Save the tilemap to a text file
 @param filename A string variable containing the name of the text file to save the map to
 */
bool CMap2D::SaveMap(string filename, const unsigned int uiCurLevel)
{
	// Update the rapidcsv::Document from arrMapInfo
	for (unsigned int uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
	{
		for (unsigned int uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; uiCol++)
		{
			doc.SetCell(uiCol, uiRow, arrMapInfo[uiCurLevel][uiRow][uiCol].value);
		}
		cout << endl;
	}

	// Save the rapidcsv::Document to a file
	doc.Save(FileSystem::getPath(filename).c_str());

	return true;
}

/**
@brief Find the indices of a certain value in arrMapInfo
@param iValue A const int variable containing the row index of the found element
@param iRow A const int variable containing the row index of the found element
@param iCol A const int variable containing the column index of the found element
@param bInvert A const bool variable which indicates if the row information is inverted
*/
bool CMap2D::FindValue(const int iValue, unsigned int& uirRow, unsigned int& uirCol, const bool bInvert)
{
	for (unsigned int uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
	{
		for (unsigned int uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; uiCol++)
		{
			if (arrMapInfo[uiCurLevel][uiRow][uiCol].value == iValue)
			{
				if (bInvert) uirRow = cSettings->NUM_TILES_YAXIS - uiRow - 1;
				else uirRow = uiRow;
				uirCol = uiCol;
				return true;	// Return true immediately if the value has been found
				
			}
		}
	}
	return false;
}

/**
 @brief Set current level
 */
void CMap2D::SetCurrentLevel(unsigned int uiCurLevel)
{
	if (uiCurLevel < uiNumLevels)
	{
		this->uiCurLevel = uiCurLevel;
	}
}
/**
 @brief Get current level
 */
unsigned int CMap2D::GetCurrentLevel(void) const
{
	return uiCurLevel;
}

/**
 @brief Render a tile at a position based on its tile index
 @param iRow A const int variable containing the row index of the tile
 @param iCol A const int variable containing the column index of the tile
 */
void CMap2D::RenderTile(const unsigned int uiRow, const unsigned int uiCol)
{
	if (arrMapInfo[uiCurLevel][uiRow][uiCol].value != 0)
	{
		//if (arrMapInfo[uiCurLevel][uiRow][uiCol].value < 3)
		glBindTexture(GL_TEXTURE_2D, MapOfTextureIDs.at(arrMapInfo[uiCurLevel][uiRow][uiCol].value));

		glBindVertexArray(VAO);
		//CS: Render the tile
		quadMesh->Render();
		glBindVertexArray(0);
	}
}

std::vector<glm::vec2> CMap2D::PathFind(const glm::vec2& startPos, const glm::vec2& targetPos, HeuristicFunction heuristicFunc, int weight)
{
	// Check if the startPos and targetPost are blocked
	if (isBlocked(startPos.y, startPos.x) ||
		(isBlocked(targetPos.y, targetPos.x)))
	{
		cout << "Invalid start or target position." << endl;
		// Return an empty path
		std::vector<glm::vec2> path;
		return path;
	}

	// Set up the variables and lists
	m_startPos = startPos;
	m_targetPos = targetPos;
	m_weight = weight;
	m_heuristic = std::bind(heuristicFunc, _1, _2, _3);

	// Reset AStar lists
	ResetAStarLists();

	// Add the start pos to 2 lists
	m_cameFromList[ConvertTo1D(m_startPos)].parent = m_startPos;
	m_openList.push(Grid(m_startPos, 0));

	unsigned int fNew, gNew, hNew;
	glm::vec2 currentPos;

	// Start the path finding...
	while (!m_openList.empty())
	{
		// Get the node with the least f value
		currentPos = m_openList.top().pos;
		//cout << endl << "*** New position to check: " << currentPos.x << ", " << currentPos.y << endl;
		//cout << "*** targetPos: " << m_targetPos.x << ", " << m_targetPos.y << endl;

		// If the targetPos was reached, then quit this loop
		if (currentPos == m_targetPos)
		{
			//cout << "=== Found the targetPos: " << m_targetPos.x << ", " << m_targetPos.y << endl;
			while (m_openList.size() != 0)
				m_openList.pop();
			break;
		}

		m_openList.pop();
		m_closedList[ConvertTo1D(currentPos)] = true;

		// Check the neighbors of the current node
		for (unsigned int i = 0; i < m_nrOfDirections; ++i)
		{
			const auto neighborPos = currentPos + m_directions[i];
			const auto neighborIndex = ConvertTo1D(neighborPos);

			//cout << "\t#" << i << ": Check this: " << neighborPos.x << ", " << neighborPos.y << ":\t";
			if (!isValid(neighborPos) ||
				isBlocked(neighborPos.y, neighborPos.x) ||
				m_closedList[neighborIndex] == true)
			{
				//cout << "This position is not valid. Going to next neighbour." << endl;
				continue;
			}

			gNew = m_cameFromList[ConvertTo1D(currentPos)].g + 1;
			hNew = m_heuristic(neighborPos, m_targetPos, m_weight);
			fNew = gNew + hNew;

			if (m_cameFromList[neighborIndex].f == 0 || fNew < m_cameFromList[neighborIndex].f)
			{
				//cout << "Adding to Open List: " << neighborPos.x << ", " << neighborPos.y;
				cout << ". [ f : " << fNew << ", g : " << gNew << ", h : " << hNew << "]" << endl;
				m_openList.push(Grid(neighborPos, fNew));
				m_cameFromList[neighborIndex] = { neighborPos, currentPos, fNew, gNew, hNew };
			}
			else
			{
				//cout << "Not adding this" << endl;
			}
		}
		//system("pause");
	}

	return BuildPath();
}


/**
 @brief Build a path
 */
std::vector<glm::vec2> CMap2D::BuildPath() const
{
	std::vector<glm::vec2> path;
	auto currentPos = m_targetPos;
	auto currentIndex = ConvertTo1D(currentPos);

	while (!(m_cameFromList[currentIndex].parent == currentPos))
	{
		path.push_back(currentPos);
		currentPos = m_cameFromList[currentIndex].parent;
		currentIndex = ConvertTo1D(currentPos);
	}

	// If the path has only 1 entry, then it is the the target position
	if (path.size() == 1)
	{
		// if m_startPos is next to m_targetPos, then having 1 path point is OK
		if (m_nrOfDirections == 4)
		{
			if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 1)
				path.clear();
		}
		else
		{
			if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 2)
				path.clear();
			else if (abs(m_targetPos.y - m_startPos.y) + abs(m_targetPos.x - m_startPos.x) > 1)
				path.clear();
		}
	}
	else
		std::reverse(path.begin(), path.end());

	return path;
}

/**
 @brief Toggle the checks for diagonal movements
 */
void CMap2D::SetDiagonalMovement(const bool bEnable)
{
	m_nrOfDirections = (bEnable) ? 8 : 4;
}

/**
 @brief Print out the details about this class instance in the console
 */
void CMap2D::PrintSelf(void) const
{
	cout << endl << "AStar::PrintSelf()" << endl;

	for (unsigned uiLevel = 0; uiLevel < uiNumLevels; uiLevel++)
	{
		cout << "Level: " << uiLevel << endl;
		for (unsigned uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
		{
			for (unsigned uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; uiCol++)
			{
				cout.fill('0');
				cout.width(3);
				cout << arrMapInfo[uiLevel][uiRow][uiCol].value;
				if (uiCol != cSettings->NUM_TILES_XAXIS - 1)
					cout << ", ";
				else
					cout << endl;
			}
		}
	}

	cout << "m_openList: " << m_openList.size() << endl;
	cout << "m_cameFromList: " << m_cameFromList.size() << endl;
	cout << "m_closedList: " << m_closedList.size() << endl;

	cout << "===== AStar::PrintSelf() =====" << endl;
}

/**
 @brief Check if a position is valid
 */
bool CMap2D::isValid(const glm::vec2& pos) const
{
	//return (pos.x >= 0) && (pos.x < m_dimensions.x) &&
	//	(pos.y >= 0) && (pos.y < m_dimensions.y);
	return (pos.x >= 0) && (pos.x < cSettings->NUM_TILES_XAXIS) &&
		(pos.y >= 0) && (pos.y < cSettings->NUM_TILES_YAXIS);
}

/**
 @brief Check if a grid is blocked
 */
bool CMap2D::isBlocked(const unsigned int uiRow, const unsigned int uiCol, const bool bInvert) const
{
	if (bInvert == true)
	{
		if ((arrMapInfo[uiCurLevel][cSettings->NUM_TILES_YAXIS - uiRow - 1][uiCol].value >= 100) &&
			(arrMapInfo[uiCurLevel][cSettings->NUM_TILES_YAXIS - uiRow - 1][uiCol].value < 200))
			return true;
		else
			return false;
	}
	else
	{
		if ((arrMapInfo[uiCurLevel][uiRow][uiCol].value >= 100) &&
			(arrMapInfo[uiCurLevel][uiRow][uiCol].value < 200))
			return true;
		else
			return false;
	}
}

/**
 @brief Returns a 1D index based on a 2D coordinate using row-major layout
 */
int CMap2D::ConvertTo1D(const glm::vec2& pos) const
{
	//return (pos.y * m_dimensions.x) + pos.x;
	return (pos.y * cSettings->NUM_TILES_XAXIS) + pos.x;
}

/**
 @brief Delete AStar lists
 */
bool CMap2D::DeleteAStarLists(void)
{
	// Delete m_openList
	while (m_openList.size() != 0)
		m_openList.pop();
	// Delete m_cameFromList
	m_cameFromList.clear();
	// Delete m_closedList
	m_closedList.clear();

	return true;
}

/**
 @brief Reset AStar lists
 */
bool CMap2D::ResetAStarLists(void)
{
	// Delete m_openList
	while (m_openList.size() != 0)
		m_openList.pop();
	// Reset m_cameFromList
	for (int i = 0; i < m_cameFromList.size(); i++)
	{
		m_cameFromList[i].pos = glm::vec2(0, 0);
		m_cameFromList[i].parent = glm::vec2(0, 0);
		m_cameFromList[i].f = 0;
		m_cameFromList[i].g = 0;
		m_cameFromList[i].h = 0;
	}
	// Reset m_closedList
	for (int i = 0; i < m_closedList.size(); i++)
	{
		m_closedList[i] = false;
	}

	return true;
}


/**
 @brief manhattan calculation method for calculation of h
 */
unsigned int heuristic::manhattan(const glm::vec2& v1, const glm::vec2& v2, int weight)
{
	glm::vec2 delta = v2 - v1;
	return static_cast<unsigned int>(weight * (delta.x + delta.y));
}

/**
 @brief euclidean calculation method for calculation of h
 */
unsigned int heuristic::euclidean(const glm::vec2& v1, const glm::vec2& v2, int weight)
{
	glm::vec2 delta = v2 - v1;
	return static_cast<unsigned int>(weight * sqrt((delta.x * delta.x) + (delta.y * delta.y)));
}