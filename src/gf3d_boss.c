
#include "simple_logger.h"

#include "gf3d_enemy.h"
#include "gf3d_player.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

void gf3d_boss_init()
{

}
void gf3d_boss_think(Entity *self)
{
	if (!self)return;
	gfc_matrix_translate(
		self->modelMatrix,
		vector3d(-0.1, 0, 0));
	gfc_matrix_rotate(
		self->modelMatrix,
		self->modelMatrix,
		0.01,
		vector3d(0, 1, 0));
	if (self->modelMatrix[3][0] <= -100)
	{
		gfc_matrix_translate(
			self->modelMatrix,
			vector3d(200, 0, 0));
	}
	if (location().x - self->modelMatrix[3][0] <= 4 && location().y - self->modelMatrix[3][1] <= 4 && location().z - self->modelMatrix[3][2] <= 4 && location().x - self->modelMatrix[3][0] >= -4 && location().y - self->modelMatrix[3][1] >= -4 && location().z - self->modelMatrix[3][2] >= -4)
	{
		gf3d_entity_free(self);
		jumpoff();
	}
}