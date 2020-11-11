
#include "simple_logger.h"

#include "gf3d_player.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

enum form{ stealth = 0, platformer = 1 };
typedef struct
{
	enum form form;			/**Stealth or Platformer?*/
	float speed;			/**It's a mystery what this one does*/
	float jumpheight;		/**Self explanatory, again, have fun*/
	enum state crouch;		/**Is the player crouching?*/
	enum state prone;		/**Is the player prone?*/
	enum state sprint;		/**Is the player sprinting?*/
	enum state cloaking;	/**Is the player using cloaking technology?*/
	enum state boxxed;		/**Is the player using the most cutting-edge of stealth technology?*/
	float sprintspeed;		/**Sprint speed, AKA the speed when you sprint.*/
	float gravity;			/**Player gravity.*/
	enum state grounded;	/**Is the player on the ground?*/
	float tpdistance;		/**Teleportation distance*/
	//float flightspeed;		/**Like speed, but upwards*/
}Player;
Player player;
const Uint8 * keys;
float airvelocity;
float djwait; //Double Jump Wait, the time until a double jump is activated, to avoid the player instantly wasting it.
float formwait;
enum state cantp;
float cloakwait;
float boxwait;
float tpwait;
float originspeed;
void gf3d_player_init()
{
	if (player.form != NULL)
	{
		slog("Uh oh, the player already exists in the world.");
		return;
	}
	keys = SDL_GetKeyboardState(NULL);
	player.form = platformer;
	player.speed = 0.005;
	player.jumpheight = 0.01;
	player.crouch = no;
	player.prone = no;
	player.sprint = no;
	player.cloaking = no;
	player.boxxed = no;
	player.sprintspeed = 0.01;
	player.gravity = 0.00001;
	player.grounded = no;
	player.tpdistance = 10;
	airvelocity = 0;
	djwait = 1;
	formwait = 1;
	cloakwait = 1;
	boxwait = 1;
	tpwait = 1;
	cantp = yes;
	originspeed = player.speed;
}

void gf3d_player_think(Entity *self)
{
	if (!self)return;
	gf3d_entity_set_colliders(self, (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	if (player.form == platformer)
	{
		
	}
	if (player.form == stealth)
	{

	}
	if (player.crouch == yes)
	{

	}
	if (player.crouch == yes)
	{

	}
	if (player.crouch == yes)
	{

	}
	if (player.crouch == yes)
	{

	}
	/*if (self->modelMatrix[3][2] <= 0)
	{
		player.grounded = yes;
	}
	else
	{
		player.grounded = no;
	}*/
	if (formwait > 0)
	{
		formwait -= 0.001;
	}
	if (cloakwait > 0)
	{
		cloakwait -= 0.001;
	}
	if (boxwait > 0)
	{
		boxwait -= 0.001;
	}
	if (tpwait > 0)
	{
		tpwait -= 0.001;
	}
	if (keys[SDL_SCANCODE_TAB])
	{
		if (formwait <= 0)
		{
			formwait = 1;
			if (player.form == platformer)
			{
				player.form = stealth;
				self->modelMatrix[1][2] = 0.9;
				//self->model = gf3d_model_load("cloaked");
				player.speed = originspeed * 0.5;
			}
			else if (player.form == stealth)
			{
				player.form = platformer;
				self->modelMatrix[1][2] = 1;
				//->model = gf3d_model_load("robo");
				player.speed = originspeed;
				player.crouch = no;
				player.prone = no;
				player.cloaking = no;
				player.boxxed = no;
			}
		}
	}
	if (keys[SDL_SCANCODE_LSHIFT])
	{
		if (player.form == platformer)
		{
			player.sprint = yes;
		}
		else if (player.form == stealth)
		{
			player.crouch = yes;
			self->modelMatrix[1][2] = 0.5;
			player.speed = originspeed * 0.3;
		}
	}
	else
	{
		if (player.form == platformer)
		{
			player.sprint = no;
		}
		else if (player.form == stealth)
		{
			if (player.crouch == yes)
			{
				player.crouch = no;
				self->modelMatrix[1][2] = 0.9;
				player.speed = originspeed * 0.5;
			}
		}
	}
	if (keys[SDL_SCANCODE_LCTRL])
	{
		if (player.form == stealth)
		{
			if (player.crouch == yes)
			{
				player.crouch = no;
			}
			player.prone = yes;
			self->modelMatrix[1][2] = 0.2;
			player.speed = originspeed * 0.1;
		}
	}
	else
	{
		if (player.form == stealth)
		{
			if (player.prone == yes)
			{
				player.prone = no;
				self->modelMatrix[1][2] = 0.9;
				player.speed = originspeed * 0.5;
			}
		}
	}
	if (keys[SDL_SCANCODE_A])
	{
		if (self->canx == yes)
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
		else
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
	}
	if (keys[SDL_SCANCODE_D])
	{
		if (self->cannx == yes)
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
		else
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
	}
	if (keys[SDL_SCANCODE_S])
	{
		if (self->cany == yes)
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
		else
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
	}
	if (keys[SDL_SCANCODE_W])
	{
		if (self->canny == yes)
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
		else
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

	}
	if (keys[SDL_SCANCODE_X])
	{
		if (player.form == platformer)
		{
			airvelocity += player.gravity + 0.000005;
		}
		if (player.form == stealth)
		{
			if (cloakwait <= 0)
			{
				cloakwait = 1;
				if (player.cloaking == no)
				{
					player.cloaking = yes;
					self->model = gf3d_model_load("cloaked");
				}
				else if (player.cloaking == yes)
				{
					player.cloaking = no;
					self->model = gf3d_model_load("robo");
				}
			}
		}
	}
	if (keys[SDL_SCANCODE_LALT])
	{
		if (player.form == platformer)
		{

			if (keys[SDL_SCANCODE_A])
			{
				if (tpwait <= 0)
				{
					tpwait = 1;
					gfc_matrix_translate(
						self->modelMatrix,
						vector3d(player.tpdistance, 0, 0));
					cantp = no;
				}
			}
			if (keys[SDL_SCANCODE_D])
			{
				if (tpwait <= 0)
				{
					tpwait = 1;
					gfc_matrix_translate(
						self->modelMatrix,
						vector3d(-player.tpdistance, 0, 0));
					cantp = no;
				}
			}
			if (keys[SDL_SCANCODE_S])
			{
				if (tpwait <= 0)
				{
					tpwait = 1;
					gfc_matrix_translate(
						self->modelMatrix,
						vector3d(0, player.tpdistance, 0));
					cantp = no;
				}
			}
			if (keys[SDL_SCANCODE_W])
			{
				if (tpwait <= 0)
				{
					tpwait = 1;
					gfc_matrix_translate(
						self->modelMatrix,
						vector3d(0, -player.tpdistance, 0));
					cantp = no;
				}
			}
		}
		if (player.form == stealth)
		{
			if (boxwait <= 0)
			{
				boxwait = 1;
				if (player.boxxed == no)
				{
					player.boxxed = yes;
					player.cloaking = no;
					self->model = gf3d_model_load("box");
				}
				else if (player.boxxed == yes)
				{
					player.boxxed = no;
					self->model = gf3d_model_load("robo");
				}
			}
		}
	}
	if (player.grounded == yes)
	{
		airvelocity = 0;
		djwait = 1;
		cantp = yes;
	}
	if (player.grounded == yes && keys[SDL_SCANCODE_SPACE])
	{
		if (player.crouch == no && player.prone == no)
		{
			player.grounded = no;
			airvelocity = player.jumpheight;
		}
	}
	if (player.grounded == no)
	{
		gfc_matrix_translate(
			self->modelMatrix,
			vector3d(0, 0, airvelocity));
		if (airvelocity < 0.1)
		{
			airvelocity -= player.gravity;
		}
		if (djwait != 10)
		{
			djwait -= 0.001;
		}
		
	}
	if (player.grounded == no && keys[SDL_SCANCODE_SPACE] && djwait <= 0 && player.form == platformer)
	{
		airvelocity = player.jumpheight;
		djwait = 10;
	}
	if (self->modelMatrix[3][2] <= -100 || self->modelMatrix[3][1] <= -100 || self->modelMatrix[3][0] <= -100 || self->modelMatrix[3][2] >= 100 || self->modelMatrix[3][1] >= 100 || self->modelMatrix[3][0] >= 100)
	{
		self->modelMatrix[3][0] = 0;
		self->modelMatrix[3][1] = 0;
		self->modelMatrix[3][2] = 5;
		airvelocity = 0;
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