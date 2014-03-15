//
//  gamelogic.h
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#ifndef GyroMaze_gamelogic_h
#define GyroMaze_gamelogic_h

#include "common.h"
#include "mazegeneration.h"

/** //////////////////////////////////////////////////////
 DRAWING VARIABLES
 */

extern int timeSum;
extern const int kFrameRate;
extern Window *game_window;
extern AppTimer *timer;

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window);
void select_pop_click_handler(ClickRecognizerRef recognizer, Window *window);
void handle_accel(AccelData *accel_data, uint32_t num_samples);
void config_provider(Window *window);
void window_load(Window *window);
void window_unload(Window *window);

void restart(void);
void draw_maze(GContext *ctx, int maze[kMaxData][kMaxData], int maze_size);
void make_maze(void);
void disc_init(Disc *disc);
void disc_apply_force(Disc *disc, Vec2d force);
void disc_apply_accel(Disc *disc, AccelData accel);
void disc_update(Disc *disc);

void disc_layer_update_callback(Layer *me, GContext *ctx);
void timer_callback(void *data);

#endif
