
#include "simple_logger.h"

#include "gf3d_player.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

enum state{yes=0, no=1};
typedef struct
{
	Uint8 form;				/**1 or 2, 1 is stealth mode while 2 is platformer mode. Anything beyond that won't do anything.*/
	float speed;			/**It's a mystery what this one does*/
	float jumpheight;		/**Self explanatory, again, have fun*/
	enum state crouch;		/**Is the player crouching?*/
	enum state prone;		/**Is the player prone?*/
	enum state sprint;		/**Is the player sprinting?*/
	float sprintspeed;		/**Sprint speed, AKA the speed when you sprint.*/
	float gravity;			/**Player gravity.*/
	enum state grounded;	/**Is the player on the ground?*/
}Player;
Player player;
const Uint8 * keys;
float airvelocity;
float djwait; //Double Jump Wait, the time until a double jump is activated, to avoid the player instantly wasting it.
void gf3d_player_init()
{
	if (player.form != NULL)
	{
		slog("Uh oh, the player already exists in the world.");
		return;
	}
	keys = SDL_GetKeyboardState(NULL);
	player.form = 1;
	player.speed = 0.005;
	player.jumpheight = 0.01;
	player.crouch = no;
	player.prone = no;
	player.sprint = no;
	player.sprintspeed = 0.01;
	player.gravity = 0.00001;
	player.grounded = no;
	airvelocity = 0;
	djwait = 1;
}

void gf3d_player_think(Entity *self)
{
	if (!self)return;
	gf3d_entity_set_colliders(self, (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	/*if (self->modelMatrix[3][2] <= 0)
	{
		player.grounded = yes;
	}
	else
	{
		player.grounded = no;
	}*/
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		player.sprint = yes;
	}
	else
	{
		player.sprint = no;
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (player.sprint == yes)
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(player.sprintspeed, 0, 0));
		}
		else
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(player.speed, 0, 0));
		}
		
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (player.sprint == yes)
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(-player.sprintspeed, 0, 0));
		}
		else
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(-player.speed, 0, 0));
		}
	}
	if (keys[SDL_SCANCODE_S])
	{
		if (player.sprint == yes)
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(0, player.sprintspeed, 0));
		}
		else
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(0, player.speed, 0));
		}
	}
	if (keys[SDL_SCANCODE_W])
	{
		if (player.sprint == yes)
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(0, -player.sprintspeed, 0));
		}
		else
		{
			gfc_matrix_translate(
				self->modelMatrix,
				vector3d(0, -player.speed, 0));
		}
	}
	if (player.grounded == yes)
	{
		airvelocity = 0;
	}
	if (player.grounded == yes && keys[SDL_SCANCODE_SPACE])
	{
		player.grounded = no;
		airvelocity = player.jumpheight;
		djwait = 1;
	}
	if (player.grounded == no)
	{
		gfc_matrix_translate(
			self->modelMatrix,
			vector3d(0, 0, airvelocity));
		airvelocity -= player.gravity;
		if (djwait != 10)
		{
			djwait -= 0.001;
		}
		
	}
	if (player.grounded == no && keys[SDL_SCANCODE_SPACE] && djwait <= 0)
	{
		airvelocity = player.jumpheight;
		djwait = 10;
	}
	
}
void gf3d_player_grounded()
{
	player.grounded = yes;
}
void gf3d_player_air()
{
	player.grounded = no;
}