
#include "simple_logger.h"

#include "gf3d_enemy.h"
#include "gf3d_player.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

typedef struct
{
	float speed;			/**It's a mystery what this one does*/
	float jumpheight;		/**Self explanatory, again, have fun*/
	float gravity;			/**Enemy gravity.*/
	enum state hostile;		/**Is the enemy hostile?*/
	float vel;

}Enemy;
Enemy enemy;
Entity *ent[10] = { 0 };
void gf3d_enemy_init()
{
	enemy.gravity = 0.00001;
	enemy.hostile = no;
	ent[0] = gf3d_entity_new();
	ent[0]->model = gf3d_model_load("platform1");
}
void gf3d_enemy_think(Entity *self)
{
	if (!self)return;
	if (self->type == 2)
	{
		gf3d_entity_set_colliders(self, (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
		if (enemy.hostile == yes)
		{
			gfc_matrix_rotate(
				self->modelMatrix,
				self->modelMatrix,
				0.01,
				vector3d(0,	1, 0));
			if (location().x > self->modelMatrix[3][0])
			{
				gfc_matrix_translate(
					self->modelMatrix,
					vector3d(0.002, 0, 0));
			}
			if (location().x < self->modelMatrix[3][0])
			{
				gfc_matrix_translate(
					self->modelMatrix,
					vector3d(-0.002, 0, 0));
			}
			if (location().y > self->modelMatrix[3][1])
			{
				gfc_matrix_translate(
					self->modelMatrix,
					vector3d(0, 0.002, 0));
			}
			if (location().y < self->modelMatrix[3][1])
			{
				gfc_matrix_translate(
					self->modelMatrix,
					vector3d(0, -0.002, 0));
			}
			if (location().x - self->modelMatrix[3][0] <= 1 && location().y - self->modelMatrix[3][1] <= 1 && location().z - self->modelMatrix[3][2] <= 1 && location().x - self->modelMatrix[3][0] >= -1 && location().y - self->modelMatrix[3][1] >= -1 && location().z - self->modelMatrix[3][2] >= -1)
			{
				respawn();
			}
		}
	}
	if (self->type >= 3)
	{
		gf3d_entity_set_colliders(self, (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
		if (enemy.hostile == yes)
		{
			self->cooldown -= 0.001;
			gfc_matrix_make_translation(
				ent[0]->modelMatrix,
				vector3d(self->modelMatrix[3][0], self->modelMatrix[3][1], self->modelMatrix[3][2]));
			if (self->cooldown <= 0)
			{
				self->cooldown = 5;
				Vector2D shoot;
				if (location().x > self->modelMatrix[3][0])
				{
					shoot.x = 0.002;
				}
				if (location().x < self->modelMatrix[3][0])
				{
					shoot.x = -0.002;
				}
				if (location().y > self->modelMatrix[3][1])
				{
					shoot.y = 0.002;
				}
				if (location().y < self->modelMatrix[3][1])
				{
					shoot.y = -0.002;
				}
			}
				if (location().x - self->modelMatrix[3][0] <= 1 && location().y - self->modelMatrix[3][1] <= 1 && location().z - self->modelMatrix[3][2] <= 1 && location().x - self->modelMatrix[3][0] >= -1 && location().y - self->modelMatrix[3][1] >= -1 && location().z - self->modelMatrix[3][2] >= -1)
				{
					respawn();
				}
		}
	}
	if (self->ground == yes)
	{
		enemy.vel = 0;
	}
	if (self->ground == no)
	{
		gfc_matrix_translate(
			self->modelMatrix,
			vector3d(0, 0, enemy.vel));
		if (enemy.vel < 0.1)
		{
			enemy.vel -= enemy.gravity;
		}
	}
	if (self->modelMatrix[3][2] <= -100 || self->modelMatrix[3][1] <= -100 || self->modelMatrix[3][0] <= -100 || self->modelMatrix[3][2] >= 100 || self->modelMatrix[3][1] >= 100 || self->modelMatrix[3][0] >= 100)
	{
		self->modelMatrix[3][0] = 0;
		self->modelMatrix[3][1] = 0;
		self->modelMatrix[3][2] = 5;
		enemy.vel = 0;
	}
	
}

void gf3d_detect()
{
	enemy.hostile = yes;
}
void gf3d_lose()
{
	enemy.hostile = no;
}