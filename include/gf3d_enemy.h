#ifndef __GF3D_ENEMY_H__
#define __GF3D_ENEMY_H__

#include "gfc_types.h"
#include "gfc_matrix.h"
#include "gf3d_model.h"
#include "gf3d_entity.h"
#include "gf3d_vgraphics.h"

#include "SDL_keycode.h"
#include "SDL_events.h"

/**
* @purpose this is the file to access the the enemy
*/

void gf3d_enemy_init();
void gf3d_enemy_think(Entity *self);

void gf3d_enemy_grounded();
void gf3d_enemy_air();
void gf3d_detect();
#endif