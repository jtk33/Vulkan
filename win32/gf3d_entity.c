
#include "simple_logger.h"

#include "gf3d_entity.h"


typedef struct
{
	Entity	*entity_list;	/**<My big ole list of entities*/
	Uint32	entity_count;	/**<upper limit for concurrent active entities*/
}EntityManager;

static EntityManager gf3d_entity = {0};
void gf3d_entity_free(Entity *entity);


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
			return &gf3d_entity.entity_list[i];
		}
	}
	slog("Failed to provide new entity, no unused slots");
	return NULL;
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