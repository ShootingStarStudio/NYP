/**
 FileSystem
 @brief A class to get directory and file information
 Date: Mar 2020
 */
#pragma once

 // Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

class CRandomiser
{
public:
	static glm::vec3 GetRandomVec3(const float fRadius = 1.0f)
	{
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		float phi = ((float)rand() / (RAND_MAX)) * (2.0f * glm::pi<float>());
		//float costheta = rand() % 2 - 1.0f;		// random(-1, 1) using int randNum = rand()%(max-min + 1) + min;
		float costheta = ((float)rand() / (RAND_MAX)) * 2.0f - 1.0f;	// random(-1, 1)
		//float u = (float)(rand() % 1);			// random(0, 1) using int randNum = rand()%(max-min + 1) + min;
		float u = ((float)rand() / (RAND_MAX));	// random(0, 1)

		//const float R = 1.0f;
		float theta = acos(costheta);
		float r = fRadius * std::pow(u, 1 / 3);

		glm::vec3 randomVec3 = glm::vec3(	r * sin(theta) * cos(phi),
											r * sin(theta) * sin(phi),
											r * cos(theta));

		return randomVec3;
	}

private:
};
