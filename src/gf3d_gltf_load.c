#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_list.h"
#include "simple_json_parse.h"

#include "gf3d_gltf_load.h"

GLB_File *simple_glb_new()
{
	GLB_File *glbFile = NULL;
	glbFile = (GLB_File*)malloc(sizeof(GLB_File));
	if (!glbFile)
	{
		slog("failed to allocate space for GLP FILE");
		return NULL;
	}
	memset(glbFile, 0, sizeof(GLB_File));
	return glbFile;
}

uint32_t simple_glb_get_chunk_length(GLB_Chunk *chunk)
{
	uint32_t size = 0;
	if (!chunk)
	{
		slog("no chunk provided");
		return 0;
	}
	size = (sizeof(uint32_t)* 2) + chunk->chunkLength;
	slog("chunk is size %i", size);
	return size;
}

GLB_Chunk *simple_glb_get_chunk_by_type(GLB_File *glbFile, GLB_ChunkType type)
{
	int i;
	if (!glbFile)return NULL;
	for (i = 0; i < glbFile->chunkCount; i++)
	{
		if (glbFile->chunkList[i].chunkType == type)
		{
			return &glbFile->chunkList[i];
		}
	}
	return NULL;
}

SJson *simple_glb_get_json(GLB_File *glbFile)
{
	SJson *json = NULL;
	GLB_Chunk *chunk = NULL;
	if (!glbFile)
	{
		slog("no glbFile provided");
		return NULL;
	}
	chunk = simple_glb_get_chunk_by_type(glbFile, GLB_CT_JSON);
	if (!chunk)
	{
		slog("no JSON chunk in GLB, bad file");
		return NULL;
	}
	slog("attempting to parse json data from chunk. length %i", chunk->chunkLength);
	//    slog("json text loaded:%s\n",chunk->chunkData);
	json = sj_parse_buffer(chunk->chunkData, chunk->chunkLength);
	if (!json)
	{
		slog("json data from glb file failed to parse");
		return NULL;
	}
	return json;
}

void simple_glb_parse(GLB_File *glb)
{
	GLB_Chunk *chunk = NULL;
	uint32_t index = 0;
	uint32_t chunkCount = 0;
	uint32_t chunkIndex = 0;
	if (!glb)return;
	while (index < glb->header.length)
	{
		chunk = (GLB_Chunk *)&glb->buffer[index];
		index += simple_glb_get_chunk_length(chunk);
		chunkCount++;
	}
	slog("chunkCount: %i", chunkCount);
	glb->chunkList = (GLB_Chunk*)malloc(sizeof(GLB_Chunk)*chunkCount);
	if (!glb->chunkList)
	{
		slog("failed to allocate chunk list");
		return;
	}
	memset(glb->chunkList, 0, sizeof(GLB_Chunk)*chunkCount);
	index = 0;
	chunkIndex = 0;
	while ((index < glb->header.length) && (chunkIndex < chunkCount))
	{
		chunk = (GLB_Chunk *)&glb->buffer[index];

		glb->chunkList[chunkIndex].chunkLength = chunk->chunkLength - 4;
		glb->chunkList[chunkIndex].chunkType = chunk->chunkType;
		glb->chunkList[chunkIndex].chunkData = (char *)&chunk->chunkData;
		switch (chunk->chunkType)
		{
		case GLB_CT_JSON:
			slog("chunk %i is JSON", chunkIndex);

			break;
		case GLB_CT_BIN:
			slog("chunk %i is BINary", chunkIndex);
			break;
		default:
			slog("chunk %i is unknown: 0x%x", chunkIndex, chunk->chunkType);
		}
		chunkIndex++;
		index += simple_glb_get_chunk_length(chunk);
	}
	glb->chunkCount = chunkCount;
	slog("parsed %i chunks out of glb file", chunkCount);
}

GLB_File *simple_glb_load(char * filepath)
{
	GLB_File *glbFile = NULL;
	FILE *file;
	int32_t fileLength = 0;
	file = fopen(filepath, "rb");
	if (!file)
	{
		slog("failed to open file %s", filepath);
		return NULL;
	}
	glbFile = simple_glb_new();
	if (!glbFile)
	{
		fclose(file);
		return NULL;
	}

	fread(&glbFile->header, sizeof(GLB_Header), 1, file);

	slog("glb file header:\n-magic: %X\n-version: %i\n-length:%i", glbFile->header.magic, glbFile->header.version, glbFile->header.length);
	fileLength = glbFile->header.length - sizeof(GLB_Header);

	if ((fileLength <= 0) || ((glbFile->buffer = malloc(fileLength)) == NULL))
	{
		slog("bad glb file length: %i", fileLength);
		fclose(file);
		simple_glb_free(glbFile);
		return NULL;
	}
	memset(glbFile->buffer, 0, fileLength);

	slog("read %i bytes from glb file", fread(glbFile->buffer, fileLength, 1, file)*fileLength);

	fclose(file);

	simple_glb_parse(glbFile);
	return glbFile;
}
void gf3d_glb_get_counts_from_file(GLBData *glb, FILE* file)
{
	char buf[256];
	int  numvertices = 0;
	int  numtexels = 0;
	int  numnormals = 0;
	int  numfaces = 0;

	if ((file == NULL) ||
		(glb == NULL))
	{
		return;
	}
	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fgets(buf, sizeof(buf), file);
				numvertices++;
				break;
			case 'n':
				fgets(buf, sizeof(buf), file);
				numnormals++;
				break;
			case 't':
				fgets(buf, sizeof(buf), file);
				numtexels++;
				break;
			default:
				break;
			}
			break;
		case 'f':
			fgets(buf, sizeof(buf), file);
			numfaces++;
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
	glb->vertex_count = numvertices;
	glb->texel_count = numtexels;
	glb->normal_count = numnormals;
	glb->face_count = numfaces;
}
void gf3d_glb_load_reorg(GLBData *glb)
{
	int i, f;
	int vert = 0;
	int vertexIndex, normalIndex, texelIndex;

	if (!glb)return;

	glb->face_vert_count = glb->face_count * 3;
	glb->faceVertices = (Vertex *)gfc_allocate_array(sizeof(Vertex), glb->face_vert_count);
	glb->outFace = (Face *)gfc_allocate_array(sizeof(Face), glb->face_count);

	for (i = 0; i < glb->face_count; i++)
	{
		for (f = 0; f < 3; f++, vert++)
		{
			vertexIndex = glb->faceVerts[i].verts[f];
			normalIndex = glb->faceNormals[i].verts[f];
			texelIndex = glb->faceTexels[i].verts[f];

			vector3d_copy(glb->faceVertices[vert].vertex, glb->vertices[vertexIndex]);
			vector3d_copy(glb->faceVertices[vert].normal, glb->normals[normalIndex]);
			vector2d_copy(glb->faceVertices[vert].texel, glb->texels[texelIndex]);

			glb->outFace[i].verts[f] = vert;
		}
	}
}
void gf3d_glb_load_get_data_from_file(GLBData *glb, FILE* file)
{
	int  numvertices = 0;
	int  numnormals = 0;
	int  numtexcoords = 0;
	int  numfaces = 0;
	char buf[128];
	float x, y, z;
	int f[3][3];

	if (file == NULL)
		return;

	rewind(file);
	while (fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case 'v':
			switch (buf[1])
			{
			case '\0':
				fscanf(
					file,
					"%f %f %f",
					&x,
					&y,
					&z
					);
				glb->vertices[numvertices].x = x;
				glb->vertices[numvertices].y = y;
				glb->vertices[numvertices].z = z;
				numvertices++;
				break;
			case 'n':
				fscanf(
					file,
					"%f %f %f",
					&x,
					&y,
					&z
					);
				glb->normals[numnormals].x = x;
				glb->normals[numnormals].y = y;
				glb->normals[numnormals].z = z;
				numnormals++;
				break;
			case 't':
				fscanf(
					file,
					"%f %f",
					&x,
					&y
					);
				glb->texels[numtexcoords].x = x;
				glb->texels[numtexcoords].y = 1 - y;
				numtexcoords++;
				break;
			default:
				break;
			}
			break;
		case 'f':
			fscanf(
				file,
				"%d/%d/%d %d/%d/%d %d/%d/%d",
				&f[0][0],
				&f[0][1],
				&f[0][2],

				&f[1][0],
				&f[1][1],
				&f[1][2],

				&f[2][0],
				&f[2][1],
				&f[2][2]);

			glb->faceVerts[numfaces].verts[0] = f[0][0] - 1;
			glb->faceTexels[numfaces].verts[0] = f[0][1] - 1;
			glb->faceNormals[numfaces].verts[0] = f[0][2] - 1;

			glb->faceVerts[numfaces].verts[1] = f[1][0] - 1;
			glb->faceTexels[numfaces].verts[1] = f[1][1] - 1;
			glb->faceNormals[numfaces].verts[1] = f[1][2] - 1;

			glb->faceVerts[numfaces].verts[2] = f[2][0] - 1;
			glb->faceTexels[numfaces].verts[2] = f[2][1] - 1;
			glb->faceNormals[numfaces].verts[2] = f[2][2] - 1;
			numfaces++;
			break;
		default:
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}
void simple_glb_free(GLB_File *glbFile)
{
	if (!glbFile)
	{
		slog("no glbFile handle provided");
		return;
	}
	if (glbFile->chunkList)
	{
		free(glbFile->chunkList);
		glbFile->chunkList = NULL;
	}
	if (glbFile->buffer)
	{
		free(glbFile->buffer);
		glbFile->buffer = NULL;
	}
	free(glbFile);
}
void simple_glb_free2(GLBData *glb)
{
	if (!glb)return;

	if (glb->vertices != NULL)
	{
		free(glb->vertices);
	}
	if (glb->normals != NULL)
	{
		free(glb->normals);
	}
	if (glb->texels != NULL)
	{
		free(glb->texels);
	}

	if (glb->faceVerts != NULL)
	{
		free(glb->faceVerts);
	}
	if (glb->faceNormals != NULL)
	{
		free(glb->faceNormals);
	}
	if (glb->faceTexels != NULL)
	{
		free(glb->faceTexels);
	}

	if (glb->faceVertices != NULL)
	{
		free(glb->faceVertices);
	}

	if (glb->outFace != NULL)
	{
		free(glb->outFace);
	}

	free(glb);
}
GLBData *gf3d_glb_load_from_file(char *filename)
{
	FILE *file;
	GLBData *glb;
	file = fopen(filename, "r");
	if (!file)
	{
		slog("failed to open glb file %s", filename);
		return NULL;
	}
	glb = (GLBData*)gfc_allocate_array(sizeof(GLBData), 1);
	if (!glb)return NULL;

	gf3d_glb_get_counts_from_file(glb, file);

	glb->vertices = (Vector3D *)gfc_allocate_array(sizeof(Vector3D), glb->vertex_count);
	glb->normals = (Vector3D *)gfc_allocate_array(sizeof(Vector3D), glb->normal_count);
	glb->texels = (Vector2D *)gfc_allocate_array(sizeof(Vector2D), glb->texel_count);

	glb->faceVerts = (Face *)gfc_allocate_array(sizeof(Face), glb->face_count);
	glb->faceNormals = (Face *)gfc_allocate_array(sizeof(Face), glb->face_count);
	glb->faceTexels = (Face *)gfc_allocate_array(sizeof(Face), glb->face_count);

	gf3d_glb_load_get_data_from_file(glb, file);
	fclose(file);
	gf3d_glb_load_reorg(glb);
	return glb;
}
/*eol@eof*/