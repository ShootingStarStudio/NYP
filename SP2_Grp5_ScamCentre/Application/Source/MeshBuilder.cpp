#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "MyMath.h"
#include "Utility.h"

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//x-axis
	v.pos.Set(-lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	//y-axis
	v.pos.Set(0, -lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	//z-axis
	v.pos.Set(0, 0, -lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(0.5f * length, 0.5f * length, 0.f); v.color = color;  
	v.normal.Set(0, 0, 1); v.texCoord.Set(1, 1); //top right
	vertex_buffer_data.push_back(v); //v0
	v.pos.Set(-0.5f * length, 0.5f * length, 0.f); v.color = color;	
	v.normal.Set(0, 0, 1); v.texCoord.Set(0, 1); //top left
	vertex_buffer_data.push_back(v); //v1
	v.pos.Set(-0.5f * length, -0.5f * length, 0.f);	v.color = color; 
	v.normal.Set(0, 0, 1);	v.texCoord.Set(0, 0); //bottom left
	vertex_buffer_data.push_back(v); //v2
	v.pos.Set(0.5f * length, -0.5f * length, 0.f); v.color = color;
	v.normal.Set(0, 0, 1); v.texCoord.Set(1, 0); //bottom right
	vertex_buffer_data.push_back(v); //v3

	//tri1
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	//tri2
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string& meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);    vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);    vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);    vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(-1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, -1, 0);	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);		v.color = color; v.normal.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(1, 0, 0);	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * length, -0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);		v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);		v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);		v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, -0.5f * length);	v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);		v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, -0.5f * length, 0.5f * length);	v.color = color; v.normal.Set(0, 0, 1);	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 36; ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned numSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / numSlices;

	for (unsigned slice = 0; slice <= numSlices; ++slice)
	{
		//edge of slice
		v.pos.Set(radius * cosf(slice * radianPerSlice), 0, radius * sinf(slice * radianPerSlice));
		v.normal.Set(0, 0, 0);
		v.color = color; vertex_buffer_data.push_back(v);
		//centre of circle
		v.pos.Set(0, 0, 0);
		v.color = color; vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice <= numSlices; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStacks;
	float degreePerSlice = 360.f / numSlices;

	for (unsigned stack = 0; stack <= numStacks; ++stack)
	{
		float phi = -90.f + degreePerStack * stack;
		for (unsigned slice = 0; slice <= numSlices; ++slice)
		{
			float theta = degreePerSlice * slice;
			float x = cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
			float y = sin(Math::DegreeToRadian(phi));
			float z = cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
			v.pos.Set(radius * x, radius * y, radius * z); 
			v.color = color; v.normal.Set(x,y,z);
			v.texCoord.Set(1 - float(slice) / float(numSlices), float(stack) / float(numStacks));
			vertex_buffer_data.push_back(v);
		}
	}
	
	for (unsigned stack = 0; stack <= numStacks; ++stack)
	{
		for (unsigned slice = 0; slice <= numSlices; ++slice)
		{
			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateHemiSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStacks;
	float degreePerSlice = 180.f / numSlices;

	for (unsigned stack = 0; stack <= numStacks; ++stack)
	{
		float phi = -90.f + degreePerStack * stack;
		for (unsigned slice = 0; slice <= numSlices; ++slice)
		{
			float theta = degreePerSlice * slice;
			float x = cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
			float y = sin(Math::DegreeToRadian(phi));
			float z = cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
			v.pos.Set(radius * x, radius * y, radius * z);
			v.color = color;
			v.normal.Set(x, y, z);
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack <= numStacks; ++stack)
	{
		for (unsigned slice = 0; slice <= numSlices; ++slice)
		{
			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned noOfSlices, float height, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / noOfSlices;
	unsigned startIndex;

	// Bottom
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = -height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z);    v.color = color; v.normal.Set(0, -1, 0);   vertex_buffer_data.push_back(v);
		v.pos.Set(0, y, 0);    v.color = color; v.normal.Set(0, -1, 0);   vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
	}

	startIndex = vertex_buffer_data.size();

	// Curve
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		v.pos.Set(x, -height * 0.5f, z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
		v.pos.Set(x, height * 0.5f, z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
	}

	// Top
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);float y = height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z);    v.color = color; v.normal.Set(0, 1, 0);    vertex_buffer_data.push_back(v);
		v.pos.Set(0, y, 0);    v.color = color; v.normal.Set(0, 1, 0);    vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < noOfSlices + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
	}


	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}
Mesh* MeshBuilder::GenerateVCylinder(const std::string& meshName, Color color, unsigned noOfSlices, float height, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / noOfSlices;
	float slicehalf = noOfSlices / 2;
	unsigned startIndex;

	// Bottom
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float y = -height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z);    v.color = color; v.normal.Set(0, -1, 0);   vertex_buffer_data.push_back(v);
		v.pos.Set(0, y, 0);    v.color = color; v.normal.Set(0, -1, 0);   vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
	}

	startIndex = vertex_buffer_data.size();

	// Curve
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta);
		float z = radius * sinf(theta);

		v.pos.Set(x, -height * 0.5f, z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
		v.pos.Set(x, height * 0.5f, z); v.color = color; v.normal.Set(x, 0, z); vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
	}

	// Top
	startIndex = vertex_buffer_data.size();
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		float theta = slice * radianPerSlice;
		float x = radius * cosf(theta); float y = height * 0.5f;
		float z = radius * sinf(theta);

		v.pos.Set(x, y, z);    v.color = color; v.normal.Set(0, 1, 0);    vertex_buffer_data.push_back(v);
		v.pos.Set(0, y, 0);    v.color = color; v.normal.Set(0, 1, 0);    vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice < slicehalf + 1; ++slice)
	{
		index_buffer_data.push_back(startIndex + slice * 2 + 0);
		index_buffer_data.push_back(startIndex + slice * 2 + 1);
	}

	v.pos.Set(radius, 0.5 * height, .0f);	v.color = color;
	v.normal.Set(0, 0, -1);	vertex_buffer_data.push_back(v); //v0
	v.pos.Set(radius, -0.5 * height, .0f);	v.color = color;
	v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v); //v1
	v.pos.Set(-radius, 0.5 * -height, .0f);	v.color = color;
	v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v); //v2
	v.pos.Set(-radius, 0.5 * height, .0f);	v.color = color;
	v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v); //v3

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned numSlices, float radius, float height) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / numSlices;

	//cone bottom
	unsigned offset = 0;
	offset = index_buffer_data.size();
	for (unsigned slice = 0; slice <= numSlices; ++slice) {

		v.pos.Set(0, -height * 0.5f, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(radius * cosf(slice * radianPerSlice), -height * 0.5f, radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice <= numSlices; ++slice) {
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	//cone side
	offset = index_buffer_data.size();
	for (unsigned slice = 0; slice <= numSlices; ++slice) {
		v.pos.Set(radius * cosf(slice * radianPerSlice), -height * 0.5f, radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(height * cosf(slice * radianPerSlice), radius, height * sinf(slice * radianPerSlice));
		v.normal.Normalize();
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, height * 0.5f, 0);
		v.color = color;
		v.normal.Set(height * cosf(slice * radianPerSlice), radius, height * sinf(slice * radianPerSlice));
		v.normal.Normalize();
		vertex_buffer_data.push_back(v);
	}
	for (unsigned slice = 0; slice <= numSlices; ++slice) {
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR, float innerR) {
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	float degreePerStack = 360.f / numStack; float degreePerSlice = 360.f / numSlice;
	float x1, z1;
	float x2, y2, z2;
	for (unsigned stack = 0; stack < numStack + 1; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			z1 = outerR * cos(stack * degreePerStack); x1 = outerR * sin(stack * degreePerStack);
			z2 = (outerR + innerR * cos(slice * degreePerSlice)) * cos(stack * degreePerStack);
			y2 = innerR * sin(slice * degreePerSlice);
			x2 = (outerR + innerR * cos(slice * degreePerSlice)) * sin(stack * degreePerStack);

			v.pos.Set(x2, y2, z2); v.color = color; v.normal.Set(x2 - x1, y2, z2 - z1);  vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStack; ++stack) {
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path) {
		//Read vertices, texcoords & normals from OBJ
		std::vector<Position> vertices;
		std::vector<TexCoord> uvs;
		std::vector<Vector3> normals;
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
			return NULL;
		//Index the vertices, texcoords & normals properly
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;
		IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
		//Create the mesh and call glBindBuffer, glBufferData
		//Use triangle list and remember to set index size
		Mesh* mesh = new Mesh(meshName);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
		mesh->mode = Mesh::DRAW_TRIANGLES;
		mesh->indexSize = index_buffer_data.size();
		return mesh;
}

Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	std::vector<Material> materials;
	bool success = LoadOBJMTL(file_path.c_str(), mtl_path.c_str(),
		vertices, uvs, normals, materials);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	Mesh* mesh = new Mesh(meshName);
	for (Material& material : materials)
		mesh->materials.push_back(material);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0;
	for (unsigned row = 0; row < numRow; ++row) {
		for (unsigned col = 0; col < numCol; ++col) {
			v.pos.Set(0.5f, 0.5f, 0.f); v.texCoord.Set((col + 1) * width, (numRow - row) * height); //top right 
			vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, 0.5f, 0.f); v.texCoord.Set((col) * width, (numRow - row) * height); //top left
			vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5f, 0.f); v.texCoord.Set((col) * width, (numRow - row - 1) * height); //bottom left
			vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0.f); v.texCoord.Set((col + 1) * width, (numRow - row - 1) * height); //bottom right
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);

			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}