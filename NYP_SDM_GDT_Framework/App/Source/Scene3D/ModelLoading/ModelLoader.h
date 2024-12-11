/**
 CModelLoader
 By: Toh Da Jun
 Date: Nov 2023
 */
#pragma once

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class CModelLoader
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // Constructor
	CModelLoader(void);

    // constructor, expects a filepath to a 3D model.
    CModelLoader(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

	// Destructor
	virtual ~CModelLoader(void);

	// Init
	bool Init(void);

	// Set methods

	// Get methods

	// Update
	bool Update(const double dElapsedTime);

    // draws the model, and thus all its meshes
    void Render(void);

	// PrintSelf
	void PrintSelf(void);

protected:
    unsigned int iTextureID;

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
