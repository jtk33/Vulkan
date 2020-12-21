#ifndef __GF3D_GLTF_LOAD_H__
#define __GF3D_GLTF_LOAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "simple_json.h"
#include "gf3d_mesh.h"

typedef struct
{
	uint32_t magic;       /**<equals 0x46546C67. It is ASCII string glTF*/
	uint32_t version;     /**<indicates the version of the Binary glTF container format.*/
	uint32_t length;      /**<is the total length of the Binary glTF, including Header and all Chunks, in bytes.*/
}GLB_Header;

typedef enum
{
	GLB_CT_JSON = 0x4E4F534A,
	GLB_CT_BIN = 0x004E4942
}GLB_ChunkType;

typedef struct
{
	uint32_t chunkLength;   /**<is the length of chunkData, in bytes.*/
	uint32_t chunkType;     /**<indicates the type of chunk. one of GLB_ChunkType*/
	char    *chunkData;     /**<is a binary payload of chunk*/
}GLB_Chunk;

typedef struct
{
	GLB_Header header;      /**<file header for GLB file*/
	char * buffer;          /**<data blob for the GLB file*/
	GLB_Chunk *chunkList;   /**<list of chunks in the buffer*/
	uint32_t   chunkCount;  /**<how many chunks are in the list*/
}GLB_File;

typedef struct
{
	Vector3D *vertices;
	Uint32 vertex_count;
	Vector3D *normals;
	Uint32 normal_count;
	Vector2D *texels;
	Uint32 texel_count;

	Face *faceVerts;
	Face *faceNormals;
	Face *faceTexels;

	Uint32 face_count;

	Face *outFace;

	Vertex *faceVertices;
	Uint32  face_vert_count;
}GLBData;


/**
* @brief given a glbFile, parse the json out of it
* @param glbFile the file to parse
* @return NULL on error, or a SJson object
* @note: returned object must be freed with sj_free()
*/
SJson *simple_glb_get_json(GLB_File *glbFile);

/**
* @brief load a GLB file from disk
* @note file must be freed with simple_glb_free
* @param filepath the file to load
* @return NULL on error (see logs) or a pointer to a loaded GLB file otherwise
*/
GLB_File *simple_glb_load(char * filepath);

/**
* @brief free the data from a glbFile
* @param glbFFile the glb data to free
*/
void simple_glb_free(GLB_File *glbFile);
GLBData *gf3d_glb_load_from_file(char *filename);


#endif