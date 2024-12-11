#include "FrustumCulling.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

#include <iostream>
using namespace std;

/**
@brief Constructor
*/
CFrustumCulling::CFrustumCulling(void)
	: fLineWidth(3.0f)
{
}

/**
@brief Destructor
*/
CFrustumCulling::~CFrustumCulling(void)
{
	DeInit();
}

/**
@brief Initialise this grid
		//m_points[0] = <Left, Bottom, Near>;
		//m_points[1] = <Left, Top, Near>;
		//m_points[2] = <Right, Bottom, Near>;
		//m_points[3] = <Right, Top, Near>;
		//m_points[4] = <Left, Bottom, Far>;
		//m_points[5] = <Left, Top, Far>;
		//m_points[6] = <Right, Bottom, Far>;
		//m_points[7] = <Right, Top, Far>;
@return true if the initialisation is successful, else false.
*/
bool CFrustumCulling::Init(void)
{
	// Update the Frustum first before we setup the rendering details.
	UpdateFrustum();

	// Update the vertices
	UpdateVertices();

	// Tell OpenGL to generate a value for VAO
	glGenVertexArrays(1, &VAO);
	// Tell OpenGL to generate a value for VBO
	glGenBuffers(1, &VBO);

	// Bind the VAO to the following set of operations
	glBindVertexArray(VAO);

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Reserve the memory area in OpenGL display card first. Note the NULL variable to indicate nothing is uploaded now
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrVertices), NULL, GL_DYNAMIC_DRAW);
	// Get pointer to the memory area indicated by the VBO
	pMemory = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// Copy data into this memory area
	memcpy(pMemory, arrVertices, sizeof(arrVertices));
	// Tell OpenGL to unlink to this pointer
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Tell OpenGL to unlink to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

/**
@brief DeInitialise this instance
*/
void CFrustumCulling::DeInit(void)
{
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CFrustumCulling::Update(const double dElapsedTime)
{
	// Recalculate the frustum
	UpdateFrustum();

	// Update the vertices
	UpdateVertices();

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Get pointer to the memory area indicated by the VBO
	pMemory = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// Copy data into this memory area
	memcpy(pMemory, arrVertices, sizeof(arrVertices));
	// Tell OpenGL to unlink to this pointer
	glUnmapBuffer(GL_ARRAY_BUFFER);

	return true;
}

/**
@brief PreRender
*/
void CFrustumCulling::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);

	// Store the previous line width
	glGetFloatv(GL_LINE_WIDTH, &fPreviousLineWidth);
	
	// Set to the new line width
	glLineWidth(fLineWidth);
}

/**
@brief Render
*/
void CFrustumCulling::Render(void)
{
	CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);
	CShaderManager::GetInstance()->pActiveShader->setMat4("view", view);
	CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projection);

	// render box
	glBindVertexArray(VAO);
	//glDrawArrays(GL_LINE_LOOP, 0, 36);
	glDrawArrays(GL_LINES, 0, 36);
	glBindVertexArray(0);
}

/**
@brief PostRender
*/
void CFrustumCulling::PostRender(void)
{
	// Restore to the previous line width
	glLineWidth(fPreviousLineWidth);
}

// Check if a bounding box is visible, a.k.a, it is within this Frustum
bool CFrustumCulling::IsBoxVisible(const glm::vec3& minp, const glm::vec3& maxp) const
{
	// check box outside/inside of frustum
	for (int i = 0; i < Count; i++)
	{
		if ((glm::dot(m_planes[i], glm::vec4(minp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(maxp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(minp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(maxp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(minp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(maxp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(minp.x, maxp.y, maxp.z, 1.0f)) < 0.0) &&
			(glm::dot(m_planes[i], glm::vec4(maxp.x, maxp.y, maxp.z, 1.0f)) < 0.0))
		{
			return false;
		}
	}

	// check frustum outside/inside box
	int out;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x > maxp.x) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x < minp.x) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y > maxp.y) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y < minp.y) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z > maxp.z) ? 1 : 0); if (out == 8) return false;
	out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z < minp.z) ? 1 : 0); if (out == 8) return false;

	return true;
}

/**
@brief Check if a point is within this Frustum
@param vec3Point A const glm::vec3& variable which contains the position to check
*/ 
bool CFrustumCulling::IsPointWithin(const glm::vec3& vec3Point) const
{
	glm::vec4 vec3PointInClipSpace = projection * (view * glm::vec4(vec3Point, 1.0f));

	// If it is zero, then that means that the object is in the plane of projection; 
	// it's view-space depth is zero. And such vertices are automatically clipped by OpenGL.
	if (vec3PointInClipSpace.w > 0.0f)
	{
		//cout << "WORLD COORDINATE : " << vec3Point.x << ", " << vec3Point.y << ", " << vec3Point.z << ". ";
		//cout << "ClipSpace : " << vec3PointInClipSpace.x << ", " << vec3PointInClipSpace.y << ", " << vec3PointInClipSpace.z << ". ";
		glm::vec3 ndcSpacePos = vec3PointInClipSpace / vec3PointInClipSpace.w;
		if (((ndcSpacePos.x >= -1.0f) && (ndcSpacePos.x <= 1.0f)) &&
			((ndcSpacePos.y >= -1.0f) && (ndcSpacePos.y <= 1.0f)))
		{
			//cout << "*** ACCEPTED *** : " << ndcSpacePos.x << ", " << ndcSpacePos.y << ", " << ndcSpacePos.z << endl;
			return true;
		}
		//else
		//{
		//	cout << "*** REJECTED *** : " << ndcSpacePos.x << ", " << ndcSpacePos.y << ", " << ndcSpacePos.z << endl;
		//}
	}
	//else
	//	cout << "BEHIND CLIPSPACE" << endl;

	return false;
}

/**
@brief Update the Frustum values
*/
inline void CFrustumCulling::UpdateFrustum(void)
{
	m = glm::transpose(projection * view);
	m_planes[Left] = m[3] + m[0];
	m_planes[Right] = m[3] - m[0];
	m_planes[Bottom] = m[3] + m[1];
	m_planes[Top] = m[3] - m[1];
	m_planes[Near] = m[3] + m[2];
	m_planes[Far] = m[3] - m[2];

	glm::vec3 crosses[Combinations] = {
		glm::cross(glm::vec3(m_planes[Left]), glm::vec3(m_planes[Right])),
		glm::cross(glm::vec3(m_planes[Left]), glm::vec3(m_planes[Bottom])),
		glm::cross(glm::vec3(m_planes[Left]), glm::vec3(m_planes[Top])),
		glm::cross(glm::vec3(m_planes[Left]), glm::vec3(m_planes[Near])),
		glm::cross(glm::vec3(m_planes[Left]), glm::vec3(m_planes[Far])),
		glm::cross(glm::vec3(m_planes[Right]), glm::vec3(m_planes[Bottom])),
		glm::cross(glm::vec3(m_planes[Right]), glm::vec3(m_planes[Top])),
		glm::cross(glm::vec3(m_planes[Right]), glm::vec3(m_planes[Near])),
		glm::cross(glm::vec3(m_planes[Right]), glm::vec3(m_planes[Far])),
		glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Top])),
		glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Near])),
		glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Far])),
		glm::cross(glm::vec3(m_planes[Top]), glm::vec3(m_planes[Near])),
		glm::cross(glm::vec3(m_planes[Top]), glm::vec3(m_planes[Far])),
		glm::cross(glm::vec3(m_planes[Near]), glm::vec3(m_planes[Far]))
	};

	m_points[0] = intersection<Left, Bottom, Near>(crosses);
	m_points[1] = intersection<Left, Top, Near>(crosses);
	m_points[2] = intersection<Right, Bottom, Near>(crosses);
	m_points[3] = intersection<Right, Top, Near>(crosses);
	m_points[4] = intersection<Left, Bottom, Far>(crosses);
	m_points[5] = intersection<Left, Top, Far>(crosses);
	m_points[6] = intersection<Right, Bottom, Far>(crosses);
	m_points[7] = intersection<Right, Top, Far>(crosses);

}

/**
 @brief Update the vertices of the Frustum
 */
inline void CFrustumCulling::UpdateVertices(void)
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	arrVertices[0] = m_points[0].x;
	arrVertices[1] = m_points[0].y;
	arrVertices[2] = m_points[0].z;
	arrVertices[3] = vec4Colour.x;
	arrVertices[4] = vec4Colour.y;
	arrVertices[5] = vec4Colour.z;
	arrVertices[6] = m_points[2].x;
	arrVertices[7] = m_points[2].y;
	arrVertices[8] = m_points[2].z;
	arrVertices[9] = vec4Colour.x;
	arrVertices[10] = vec4Colour.y;
	arrVertices[11] = vec4Colour.z;
	arrVertices[12] = m_points[2].x;
	arrVertices[13] = m_points[2].y;
	arrVertices[14] = m_points[2].z;
	arrVertices[15] = vec4Colour.x;
	arrVertices[16] = vec4Colour.y;
	arrVertices[17] = vec4Colour.z;
	arrVertices[18] = m_points[6].x;
	arrVertices[19] = m_points[6].y;
	arrVertices[20] = m_points[6].z;
	arrVertices[21] = vec4Colour.x;
	arrVertices[22] = vec4Colour.y;
	arrVertices[23] = vec4Colour.z;
	arrVertices[24] = m_points[6].x;
	arrVertices[25] = m_points[6].y;
	arrVertices[26] = m_points[6].z;
	arrVertices[27] = vec4Colour.x;
	arrVertices[28] = vec4Colour.y;
	arrVertices[29] = vec4Colour.z;
	arrVertices[30] = m_points[4].x;
	arrVertices[31] = m_points[4].y;
	arrVertices[32] = m_points[4].z;
	arrVertices[33] = vec4Colour.x;
	arrVertices[34] = vec4Colour.y;
	arrVertices[35] = vec4Colour.z;
	arrVertices[36] = m_points[4].x;
	arrVertices[37] = m_points[4].y;
	arrVertices[38] = m_points[4].z;
	arrVertices[39] = vec4Colour.x;
	arrVertices[40] = vec4Colour.y;
	arrVertices[41] = vec4Colour.z;
	arrVertices[42] = m_points[0].x;
	arrVertices[43] = m_points[0].y;
	arrVertices[44] = m_points[0].z;
	arrVertices[45] = vec4Colour.x;
	arrVertices[46] = vec4Colour.y;
	arrVertices[47] = vec4Colour.z;
	arrVertices[48] = m_points[1].x;
	arrVertices[49] = m_points[1].y;
	arrVertices[50] = m_points[1].z;
	arrVertices[51] = vec4Colour.x;
	arrVertices[52] = vec4Colour.y;
	arrVertices[53] = vec4Colour.z;
	arrVertices[54] = m_points[3].x;
	arrVertices[55] = m_points[3].y;
	arrVertices[56] = m_points[3].z;
	arrVertices[57] = vec4Colour.x;
	arrVertices[58] = vec4Colour.y;
	arrVertices[59] = vec4Colour.z;
	arrVertices[60] = m_points[3].x;
	arrVertices[61] = m_points[3].y;
	arrVertices[62] = m_points[3].z;
	arrVertices[63] = vec4Colour.x;
	arrVertices[64] = vec4Colour.y;
	arrVertices[65] = vec4Colour.z;
	arrVertices[66] = m_points[7].x;
	arrVertices[67] = m_points[7].y;
	arrVertices[68] = m_points[7].z;
	arrVertices[69] = vec4Colour.x;
	arrVertices[70] = vec4Colour.y;
	arrVertices[71] = vec4Colour.z;
	arrVertices[72] = m_points[7].x;
	arrVertices[73] = m_points[7].y;
	arrVertices[74] = m_points[7].z;
	arrVertices[75] = vec4Colour.x;
	arrVertices[76] = vec4Colour.y;
	arrVertices[77] = vec4Colour.z;
	arrVertices[78] = m_points[5].x;
	arrVertices[79] = m_points[5].y;
	arrVertices[80] = m_points[5].z;
	arrVertices[81] = vec4Colour.x;
	arrVertices[82] = vec4Colour.y;
	arrVertices[83] = vec4Colour.z;
	arrVertices[84] = m_points[5].x;
	arrVertices[85] = m_points[5].y;
	arrVertices[86] = m_points[5].z;
	arrVertices[87] = vec4Colour.x;
	arrVertices[88] = vec4Colour.y;
	arrVertices[89] = vec4Colour.z;
	arrVertices[90] = m_points[1].x;
	arrVertices[91] = m_points[1].y;
	arrVertices[92] = m_points[1].z;
	arrVertices[93] = vec4Colour.x;
	arrVertices[94] = vec4Colour.y;
	arrVertices[95] = vec4Colour.z;
	arrVertices[96] = m_points[1].x;
	arrVertices[97] = m_points[1].y;
	arrVertices[98] = m_points[1].z;
	arrVertices[99] = vec4Colour.x;
	arrVertices[100] = vec4Colour.y;
	arrVertices[101] = vec4Colour.z;
	arrVertices[102] = m_points[0].x;
	arrVertices[103] = m_points[0].y;
	arrVertices[104] = m_points[0].z;
	arrVertices[105] = vec4Colour.x;
	arrVertices[106] = vec4Colour.y;
	arrVertices[107] = vec4Colour.z;
	arrVertices[108] = m_points[3].x;
	arrVertices[109] = m_points[3].y;
	arrVertices[110] = m_points[3].z;
	arrVertices[111] = vec4Colour.x;
	arrVertices[112] = vec4Colour.y;
	arrVertices[113] = vec4Colour.z;
	arrVertices[114] = m_points[2].x;
	arrVertices[115] = m_points[2].y;
	arrVertices[116] = m_points[2].z;
	arrVertices[117] = vec4Colour.x;
	arrVertices[118] = vec4Colour.y;
	arrVertices[119] = vec4Colour.z;
	arrVertices[120] = m_points[7].x;
	arrVertices[121] = m_points[7].y;
	arrVertices[122] = m_points[7].z;
	arrVertices[123] = vec4Colour.x;
	arrVertices[124] = vec4Colour.y;
	arrVertices[125] = vec4Colour.z;
	arrVertices[126] = m_points[6].x;
	arrVertices[127] = m_points[6].y;
	arrVertices[128] = m_points[6].z;
	arrVertices[129] = vec4Colour.x;
	arrVertices[130] = vec4Colour.y;
	arrVertices[131] = vec4Colour.z;
	arrVertices[132] = m_points[5].x;
	arrVertices[133] = m_points[5].y;
	arrVertices[134] = m_points[5].z;
	arrVertices[135] = vec4Colour.x;
	arrVertices[136] = vec4Colour.y;
	arrVertices[137] = vec4Colour.z;
	arrVertices[138] = m_points[4].x;
	arrVertices[139] = m_points[4].y;
	arrVertices[140] = m_points[4].z;
	arrVertices[141] = vec4Colour.x;
	arrVertices[142] = vec4Colour.y;
	arrVertices[143] = vec4Colour.z;
}

template<CFrustumCulling::Planes a, CFrustumCulling::Planes b, CFrustumCulling::Planes c>
inline glm::vec3 CFrustumCulling::intersection(const glm::vec3* crosses) const
{
	float D = glm::dot(glm::vec3(m_planes[a]), crosses[ij2k<b, c>::k]);
	glm::vec3 res = glm::mat3(crosses[ij2k<b, c>::k], -crosses[ij2k<a, c>::k], crosses[ij2k<a, b>::k]) *
		glm::vec3(m_planes[a].w, m_planes[b].w, m_planes[c].w);
	return res * (-1.0f / D);
}

/**
@brief PrintSelf
*/
void CFrustumCulling::PrintSelf(void)
{
	cout << "CFrustumCulling::PrintSelf()" << endl;
	cout << "********************************************************************************" << endl;
	cout << "LBN: " << m_points[0].x << ", " << m_points[0].y << ", " << m_points[0].z << endl;
	cout << "LTN: " << m_points[1].x << ", " << m_points[1].y << ", " << m_points[1].z << endl;
	cout << "RBN: " << m_points[2].x << ", " << m_points[2].y << ", " << m_points[2].z << endl;
	cout << "RBN: " << m_points[3].x << ", " << m_points[3].y << ", " << m_points[3].z << endl;

	cout << "LBF: " << m_points[4].x << ", " << m_points[4].y << ", " << m_points[4].z << endl;
	cout << "LTF: " << m_points[5].x << ", " << m_points[5].y << ", " << m_points[5].z << endl;
	cout << "RBF: " << m_points[6].x << ", " << m_points[6].y << ", " << m_points[6].z << endl;
	cout << "RBF: " << m_points[7].x << ", " << m_points[7].y << ", " << m_points[7].z << endl;

	cout << "Model: " << endl << glm::to_string(model) << endl;
	cout << "View: " << endl << glm::to_string(view) << endl;
	cout << "Projection: " << endl << glm::to_string(projection) << endl;
}
