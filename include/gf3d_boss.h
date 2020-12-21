#ifndef __GF3D_BOSS_H__
#define __GF3D_BOSS_H__

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

void gf3d_boss_init();
void gf3d_boss_think(Entity *self);

#endif