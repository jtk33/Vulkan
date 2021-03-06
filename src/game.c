#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_entity.h"
#include "gf3d_player.h"
#include "gf3d_enemy.h"
#include "gf3d_boss.h"

#include "gf3d_gltf_load.h"


#include "SDL_keycode.h"
#include "SDL_events.h"
int main(int argc,char *argv[])
{
    int done = 0;
    int a, i;
    Uint8 validate = 0;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
	Entity *ent[10] = { 0 };
	Matrix4 *view;
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1600,                   //screen width
        900,                    //screen height
        vector4d(1,1,1,1),		//background color
        0,                      //fullscreen
        validate                //validation
    );
	slog_sync();
	gf3d_entity_init(256);

    // main game loop
    slog("gf3d main loop begin");
	slog_sync();
	ent[0] = gf3d_entity_new();
	ent[0]->model = gf3d_model_load("robo");
	ent[0]->think = gf3d_player_think;
	gf3d_entity_set_colliders(ent[0], (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	ent[0]->modelMatrix[3][2] = 5;
	ent[0]->modelMatrix[3][0] = 0;
	ent[0]->modelMatrix[3][0] = 0;
	ent[1] = gf3d_entity_new();
	ent[1]->model = gf3d_model_load("skybox");
	ent[2] = gf3d_entity_new();
	ent[2]->model = gf3d_model_load("base");
	gf3d_entity_set_colliders(ent[2], (float)25, (float)25, (float)0.5, (float)25, (float)25, (float)0.5);
	ent[3] = gf3d_entity_new();
	ent[3]->model = gf3d_model_load("platform1");
	gfc_matrix_make_translation(
		ent[3]->modelMatrix,
		vector3d(20, -50, 0));
	gf3d_entity_set_colliders(ent[3], (float)2, (float)2, (float)0.5, (float)2, (float)2, (float)0.5);
	ent[4] = gf3d_entity_new();
	ent[4]->model = gf3d_model_load("platform1");
	gfc_matrix_make_translation(
		ent[4]->modelMatrix,
		vector3d(-20, -50, 0));
	gf3d_entity_set_colliders(ent[4], (float)2, (float)2, (float)0.5, (float)2, (float)2, (float)0.5);
	ent[5] = gf3d_entity_new();
	ent[5]->model = gf3d_model_load("detectorg");
	gfc_matrix_make_translation(
		ent[5]->modelMatrix,
		vector3d(-30, -20, 5));
	gf3d_entity_set_colliders(ent[5], (float)50, (float)15, (float)15, (float)15, (float)15, (float)15);
	ent[6] = gf3d_entity_new();
	ent[6]->model = gf3d_model_load("melee");
	ent[6]->think = gf3d_enemy_think;
	gf3d_entity_set_colliders(ent[6], (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	gfc_matrix_make_translation(
		ent[6]->modelMatrix,
		vector3d(-20, -20, 5));
	ent[7] = gf3d_entity_new();
	ent[7]->model = gf3d_model_load("shooter");
	ent[7]->think = gf3d_enemy_think;
	gf3d_entity_set_colliders(ent[7], (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	gfc_matrix_make_translation(
		ent[7]->modelMatrix,
		vector3d(20, -20, 5));
	ent[8] = gf3d_entity_new();
	ent[8]->model = gf3d_model_load("boss");
	ent[8]->think = gf3d_boss_think;
	gf3d_entity_set_colliders(ent[8], (float)0.5, (float)0.3, (float)3.2, (float)0.72, (float)0.5, (float)0);
	gfc_matrix_make_translation(
		ent[8]->modelMatrix,
		vector3d(100, -75, 5));
	/*ent[6] = gf3d_entity_new();
	ent[6]->model = gf3d_model2_load("cube");
	gfc_matrix_make_translation(
		ent[6]->modelMatrix,
		vector3d(0, 0, 0));
	gf3d_entity_set_colliders(ent[6], (float)2, (float)2, (float)2, (float)2, (float)2, (float)2);*/
	gf3d_player_init();
	gfc_matrix_rotate(
		ent[0]->modelMatrix,
		ent[0]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	ent[0]->type = 1;
	gfc_matrix_rotate(
		ent[2]->modelMatrix,
		ent[2]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	gfc_matrix_rotate(
		ent[3]->modelMatrix,
		ent[3]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	gfc_matrix_rotate(
		ent[4]->modelMatrix,
		ent[4]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	gf3d_enemy_init();
	gfc_matrix_rotate(
		ent[6]->modelMatrix,
		ent[6]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	ent[6]->type = 2;
	ent[6]->ground = no;
	ent[6]->live = yes;
	gfc_matrix_rotate(
		ent[7]->modelMatrix,
		ent[7]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	ent[7]->type = 3;
	ent[7]->ground = no;
	ent[7]->live = yes;
	gfc_matrix_rotate(
		ent[8]->modelMatrix,
		ent[8]->modelMatrix,
		(float)1.5708,
		vector3d(1, 0, 0));
	//view = gf3d_vgraphics_get_view();
	SDL_ShowCursor(0);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //update game things here
		gf3d_entity_think_all();
		gf3d_camera_think(ent[0]);
		gf3d_collision_think(ent[2], ent[0]);
		gf3d_collision_think(ent[3], ent[0]);
		gf3d_collision_think(ent[4], ent[0]);
		gf3d_collision_think(ent[2], ent[6]);
		gf3d_collision_think(ent[3], ent[6]);
		gf3d_collision_think(ent[4], ent[6]);
		gf3d_detector_think(ent[5], ent[0]);


		/*gfc_matrix_view(
			view,
			vector3d(*view[3][0], ent[0]->modelMatrix[3][1] + 9, ent[0]->modelMatrix[3][2] + 5),
			vector3d(ent[0]->modelMatrix[3][0], ent[0]->modelMatrix[3][1], ent[0]->modelMatrix[3][2] + 3.6),
			vector3d(0, 0, 1));*/
		//gfc_matrix_copy(gf3d_vgraphics_get_view, view);
		//gfc_matrix_slog(ent[0]->modelMatrix);


        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

				gf3d_entity_draw_all(bufferFrame, commandBuffer);

            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
