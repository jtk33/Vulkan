
#include "simple_logger.h"

#include "gf3d_entity.h"
#include "gf3d_player.h"

typedef struct
{
	Entity	*entity_list;	/**<My big ole list of entities*/
	Uint32	entity_count;	/**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager gf3d_entity = {0};
void gf3d_entity_free(Entity *entity);
float collidercount = 3;
float relook = 1;
float hostile = 0;
void gf3d_entity_close( void )
{
	int i;
	if (gf3d_entity.entity_list != NULL)
	{
		for (i = 0; i < (int)gf3d_entity.entity_count; i++)
		{
			gf3d_entity_free(&gf3d_entity.entity_list[i]);
		}
		free(gf3d_entity.entity_list);
	}
	memset(&gf3d_entity, 0, sizeof(EntityManager));
	slog("Entity System Closed, pog, I suppose.");
}

void gf3d_entity_init(Uint32 maxEntities)
{
	if (gf3d_entity.entity_list != NULL)
	{
		slog("WARNING: entity system already initialized");
		return;
	}
	gf3d_entity.entity_list = gfc_allocate_array(sizeof(Entity),maxEntities);
	if (!gf3d_entity.entity_list)
	{
		slog("failed to allocate big ole entity list");
	}
	gf3d_entity.entity_count = maxEntities;
	atexit(gf3d_entity_close);
	slog("Entity System Initialized, POG!");
}

void gf3d_entity_free(Entity *entity)
{
	if (!entity)return;
	gf3d_model_free(entity->model);
	memset(entity,0,sizeof(Entity));
}

Entity *gf3d_entity_new()
{
	int i;
	for (i = 0; i < (int) gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse)
		{
			gf3d_entity.entity_list[i]._inuse = 1;
			gfc_matrix_identity(gf3d_entity.entity_list[i].modelMatrix);
			gf3d_entity.entity_list[i].x = 0;
			gf3d_entity.entity_list[i].y = 0;
			gf3d_entity.entity_list[i].z = 0;
			gf3d_entity.entity_list[i].nx = 0;
			gf3d_entity.entity_list[i].ny = 0;
			gf3d_entity.entity_list[i].nz = 0;
			gf3d_entity.entity_list[i].canx = yes;
			gf3d_entity.entity_list[i].cany = yes;
			gf3d_entity.entity_list[i].cannx = yes;
			gf3d_entity.entity_list[i].canny = yes;
			return &gf3d_entity.entity_list[i];
		}
	}
	slog("Failed to provide new entity, no unused slots");
	return NULL;
}
void gf3d_entity_think(Entity *self)
{
	if (!self)return;
	if (!self->think)return;//no think function to call
	self->think(self);
}
void gf3d_entity_think_all()
{
	int i;
	for (i = 0; i < (int)gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse)continue;
		gf3d_entity_think(&gf3d_entity.entity_list[i]);
	}
}
void gf3d_entity_set_colliders(Entity *self, float cx, float cy, float cz, float cnx, float cny, float cnz)
{
	self->x = self->modelMatrix[3][0] + cx;
	self->y = self->modelMatrix[3][1] + cy;
	self->z = self->modelMatrix[3][2] + cz;
	self->nx = self->modelMatrix[3][0] - cnx;
	self->ny = self->modelMatrix[3][1] - cny;
	self->nz = self->modelMatrix[3][2] - cnz;
}

void gf3d_collision_think(Entity *self, Entity *other)
{
	if (!self)return;

	if (self->nx < other->x && self->x > other->nx && self->ny < other->y && self->y > other->ny && self->nz < other->z && self->z > other->nz)
	{
		collidercount = 3;
		gf3d_player_grounded();
	}
	else
	{
		collidercount -= 1;
		if (collidercount <= 0)
		{
			gf3d_player_air();
			other->canx = yes;
			other->cany = yes;
			other->cannx = yes;
			other->canny = yes;
		}
		
	}
	if (self->nx < other->x && self->x > other->nx && self->ny < other->y && self->y > other->ny && self->nz < other->z && self->z > other->nz && self->z > other->nz + 0.1)
	{
		collision_detect(self, other);
	}

}
void collision_detect(Entity *self, Entity *other)
{
	if (self->nx < other->x + 0.1)
	{
		other->canx = no;
	}
	if (self->x > other->nx - 0.1)
	{
		other->cannx = no;
	}
	if (self->ny < other->y + 0.1)
	{
		other->cany = no;
	}
	if (self->y > other->ny - 0.1)
	{
		other->canny = no;
	}
}
void gf3d_detector_think(Entity *self, Entity *other)
{
	if (!self)return;
	relook -= 0.0005;
	
	if (self->nx < other->x && self->x > other->nx && self->ny < other->y && self->y > other->ny && self->nz < other->z && self->z > other->nz)
	{
		if ((gfc_crandom() + 1) * 100 < gf3d_detection() && relook <= 0)
		{
			relook = 1;
			if (hostile == 0)
			{
				//gf3d_model_delete(self->model);
				self->model = gf3d_model_load("detectorr");
				hostile = 1;
			}
			
		}
	}
	else if (hostile == 1 && relook <= 0)
	{
		self->model = gf3d_model_load("detectorg");
		hostile = 0;
	}
	
	if (relook <= 0)
	{
		relook = 1;
	}
}
void gf3d_entity_draw(Entity *self, Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	if (!self)return;
	gf3d_model_draw(self->model, bufferFrame, commandBuffer, self->modelMatrix);
}
void gf3d_entity_draw_all(Uint32 bufferFrame, VkCommandBuffer commandBuffer)
{
	int i;
	for (i = 0; i < (int)gf3d_entity.entity_count; i++)
	{
		if (!gf3d_entity.entity_list[i]._inuse)continue;
		gf3d_entity_draw(&gf3d_entity.entity_list[i], bufferFrame, commandBuffer);
	}
}
/*eol@oef*/