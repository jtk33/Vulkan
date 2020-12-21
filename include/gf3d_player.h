#ifndef __GF3D_PLAYER_H__
#define __GF3D_PLAYER_H__

#include "gfc_types.h"
#include "gfc_matrix.h"
#include "gf3d_model.h"
#include "gf3d_entity.h"
#include "gf3d_vgraphics.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

/**
* @purpose this is the file to access the the player
*/

void gf3d_player_init();
void gf3d_player_think(Entity *self);

void gf3d_player_grounded();
void gf3d_player_air();
float gf3d_detection();
Vector3D location();
#endif