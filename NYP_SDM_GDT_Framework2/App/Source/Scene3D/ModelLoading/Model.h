/**
 CModel
 By: Toh Da Jun
 Date: Nov 2023
 */
#pragma once

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class CModel
{
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    // Constructor
    CModel(void);
    CModel(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	// Destructor
	virtual ~CModel(void);

	// Init
    // initializes all the buffer objects/arrays
    bool Init(void);

	// Set methods

	// Get methods

	// Update
	bool Update(const double dElapsedTime);

    // draws the model, and thus all its meshes
    void PreRender(void);
    void Render(void);
    void PostRender(void);

	// PrintSelf
	void PrintSelf(void);

protected:
    // render data 
    unsigned int VBO, EBO;

};
