#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gfc_types.h"
#include "gfc_matrix.h"
#include "gf3d_model.h"
/**
* @purpose this is the file to access the entity management system
*/

typedef struct Entity_S
{
	Uint8	_inuse;	/**<flag to make sure entities are not re-assigned while active*/
	Model	*model;
	Matrix4	modelMatrix;
	void(*think)(struct Entity_S *self);
}Entity;

void gf3d_entity_init(Uint32 maxEntities);

Entity *gf3d_entity_new();

void gf3d_entity_free(Entity *entity);

void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer);

void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer);


#endif