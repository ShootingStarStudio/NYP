#include "vertexBufferObject.h"

/**
 @brief Constructor
 */
CVertexBufferObject::CVertexBufferObject(void)
{
	bDataUploaded = false;
	uiBuffer = 0;
}

/**
 @brief Destructor
 */
CVertexBufferObject::~CVertexBufferObject(void)
{
}

/*-----------------------------------------------

Name:	CreateVBO

Params:	a_iSize - initial size of buffer

Result:	Creates vertex buffer object.

/*---------------------------------------------*/

void CVertexBufferObject::CreateVBO(int a_iSize)
{
	glGenBuffers(1, &uiBuffer);
	vData.reserve(a_iSize);
	iSize = a_iSize;
	iCurrentSize = 0;
}

/*-----------------------------------------------

Name:	DeleteVBO

Params:	none

Result:	Releases VBO and frees all memory.

/*---------------------------------------------*/

void CVertexBufferObject::DeleteVBO(void)
{
	glDeleteBuffers(1, &uiBuffer);
	bDataUploaded = false;
	vData.clear();
}

/*-----------------------------------------------

Name:	mapBufferToMemory

Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...

Result:	Maps whole buffer data to memory and
		returns pointer to data.

/*---------------------------------------------*/

void* CVertexBufferObject::MapBufferToMemory(int iUsageHint)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(iBufferType, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	MapSubBufferToMemory

Params:	iUsageHint - GL_READ_ONLY, GL_WRITE_ONLY...
		uiOffset - data offset (from where should
					data be mapped).
		uiLength - length of data

Result:	Maps specified part of buffer to memory.

/*---------------------------------------------*/

void* CVertexBufferObject::MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength)
{
	if(!bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}

/*-----------------------------------------------

Name:	UnmapBuffer

Params:	none

Result:	Unmaps previously mapped buffer.

/*---------------------------------------------*/

void CVertexBufferObject::UnmapBuffer(void)
{
	glUnmapBuffer(iBufferType);
}

/*-----------------------------------------------

Name:	BindVBO

Params:	a_iBufferType - buffer type (GL_ARRAY_BUFFER, ...)

Result:	Binds this VBO.

/*---------------------------------------------*/

void CVertexBufferObject::BindVBO(int a_iBufferType)
{
	iBufferType = a_iBufferType;
	glBindBuffer(iBufferType, uiBuffer);
}

/*-----------------------------------------------

Name:	UploadDataToGPU

Params:	iUsageHint - GL_STATIC_DRAW, GL_DYNAMIC_DRAW...

Result:	Sends data to GPU.

/*---------------------------------------------*/

void CVertexBufferObject::UploadDataToGPU(int iDrawingHint)
{
	// Reserve the memory area in OpenGL display card first. Note the NULL variable to indicate nothing is uploaded now
	glBufferData(iBufferType, vData.size(), NULL, GL_DYNAMIC_DRAW);
	// Get pointer to the memory area indicated by the VBO
	void* pMemory = glMapBuffer(iBufferType, GL_WRITE_ONLY);
	// Copy data into this memory area
	memcpy(pMemory, &vData[0], vData.size());
	// Tell OpenGL to unlink to this pointer
	glUnmapBuffer(iBufferType);

	// Original codes to upload the vData to OpenGL
	//glBufferData(iBufferType, vData.size(), &vData[0], iDrawingHint);
	bDataUploaded = true;
	vData.clear();
}

/*-----------------------------------------------

Name:	ClearMemory

Params:	none

Result:	Clear vData.

/*---------------------------------------------*/
void CVertexBufferObject::ClearMemory(void)
{
	vData.clear();
}

/*-----------------------------------------------

Name:	AddData

Params:	ptrData - pointer to arbitrary data
		uiDataSize - data size in bytes

Result:	Adds arbitrary data to VBO.

/*---------------------------------------------*/

void CVertexBufferObject::AddData(void* ptrData, unsigned int uiDataSize)
{
	vData.insert(vData.end(), (BYTE*)ptrData, (BYTE*)ptrData+uiDataSize);
	iCurrentSize += uiDataSize;
}

/*-----------------------------------------------

Name:	GetDataPointer

Params:	none

Result:	Returns data pointer (only before uploading).

/*---------------------------------------------*/

void* CVertexBufferObject::GetDataPointer(void)
{
	if(bDataUploaded)return NULL;
	return (void*)vData[0];
}

/*-----------------------------------------------

Name:	GetBufferID

Params:	none

Result:	Returns VBO ID.

/*---------------------------------------------*/

unsigned int CVertexBufferObject::GetBufferID(void)
{
	return uiBuffer;
}

/*-----------------------------------------------

Name:	GetCurrentSize

Params:	none

Result: Returns size of data that has been added to
		the buffer object.

/*---------------------------------------------*/

int CVertexBufferObject::GetCurrentSize(void)
{
	return iCurrentSize;
}