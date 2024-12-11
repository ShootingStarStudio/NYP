#include "ProjectileLines.h"

// Include Shader Manager
#include "RenderControl/ShaderManager.h"

#include "TimeControl/FPSCounter.h"

#include <GLFW/glfw3.h>

/**
 @brief Default Constructor
 */
CProjectileLines::CProjectileLines(void)
	: vec3StartPosition(glm::vec3(0.0f, 0.0f, 0.0f))
	, vec3EndPosition(glm::vec3(1.0f, 0.0f, 0.0f))
	, vec4ProjectileLineColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
	, bIsDisplayed(true)
	, fLineWidth(10.0f)
	, fBlendValue(1.0f)
{
}

/**
 @brief Default Destructor
 */
CProjectileLines::~CProjectileLines(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

/**
 @brief Initialise this class instance
 @param _name A const std::string& variable which stores the name of the shader
*/
bool CProjectileLines::InitProjectileLines(	const std::string& _name,
											glm::vec4 vec4Colour,
											const float fBlendValue)
{
	this->SetProjectileLineShader(_name);
	this->vec3StartPosition = vec3StartPosition;
	this->vec3EndPosition = vec3EndPosition;
	this->vec4ProjectileLineColour = vec4Colour;
	this->fBlendValue = fBlendValue;


	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		//start
		0.0, 0.0, 0.0, 1.0f, 1.0f, 1.0f, 1.0f,
		//end
		1.0, 0.0, 0.0, 1.0f, 1.0f, 1.0f, 1.0f,
	};

	glGenVertexArrays(1, &projectileLinesVAO);
	glGenBuffers(1, &projectileLinesVBO);

	glBindVertexArray(projectileLinesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, projectileLinesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glLineWidth(fLineWidth);

	return true;
}

/**
 @brief Set a shader to this class instance
 @param _name The name of the Shader instance in the CShaderManager
 */
void CProjectileLines::SetProjectileLineShader(const std::string& _name)
{
	sProjectileLinesShaderName = _name;
}

/**
 @brief Set 
 @param 
 */
void CProjectileLines::SetStartPosition(glm::vec3 vec3StartPosition)
{
	this->vec3StartPosition = vec3StartPosition;
}

/**
 @brief Set
 @param
 */
void CProjectileLines::SetEndPosition(glm::vec3 vec3EndPosition)
{
	this->vec3EndPosition = vec3EndPosition;
}

/**
 @brief Set the fBlendValue
 @param fBlendValue A const float variable containing the new fBlendValue
 */
void CProjectileLines::SetMaxBlendValue(const float fBlendValue)
{
	this->fBlendValue = fBlendValue;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CProjectileLines::Update(const double dElapsedTime)
{
	// Update fBlendValue
	if (bIsDisplayed)
	{
		fBlendValue -= dElapsedTime;
		if (fBlendValue < 0.1f)
			fBlendValue = 0.1f;

		// Create transformations
		// Initialize matrix to identity matrix
		projectileLineModel = glm::mat4(1.0f);

		// Translate to the start position of the projectile
		projectileLineModel = glm::translate(projectileLineModel, vec3StartPosition);

		// Rotate the line in the direction of the projectile
		const glm::vec3 a = vec3EndPosition - vec3StartPosition;	// The direction of the projectile
		const glm::vec3 b = glm::vec3(1.0f, 0.0f, 0.0f);			// The default direction of the line
		glm::vec3 v = glm::cross(b, a);
		float angle = acos(glm::dot(b, a) / (glm::length(b) * glm::length(a)));
		projectileLineModel = glm::rotate(projectileLineModel, angle, v);

		// Scale the line so it begins at the start of the projectile until the end
		projectileLineModel = glm::scale(projectileLineModel, glm::vec3(glm::length(vec3EndPosition - vec3StartPosition)-1.0f));
	}

	return true;
}

/**
 @brief PreRender
 */
void CProjectileLines::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use the shader defined for this class
	CShaderManager::GetInstance()->Use(sProjectileLinesShaderName);
}

/**
 @brief Render
 */
void CProjectileLines::Render(void)
{
	if (!bIsDisplayed)
		return;

	CShaderManager::GetInstance()->pActiveShader->setMat4("model", projectileLineModel);
	CShaderManager::GetInstance()->pActiveShader->setMat4("view", projectileLineView);
	CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projectileLineProjection);
	CShaderManager::GetInstance()->pActiveShader->setFloat("fBlendValue", fBlendValue);

	// render box
	glBindVertexArray(projectileLinesVAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

/**
 @brief PostRender
 */
void CProjectileLines::PostRender(void)
{
	// Deactivate blending mode
	glDisable(GL_BLEND);
}
