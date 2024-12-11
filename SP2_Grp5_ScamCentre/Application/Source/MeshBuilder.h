#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float length);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float length);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlices, float radius);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float radius);
	static Mesh* GenerateHemiSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float radius);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);
	static Mesh* GenerateVCylinder(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);		//split vertically
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned nemRow, unsigned numCol);


};

#endif