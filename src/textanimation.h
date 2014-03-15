//
//  textanimation.h
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#ifndef GyroMaze_textanimation_h
#define GyroMaze_textanimation_h

#include "pebble.h"

extern PropertyAnimation *prop_animation;
extern TextLayer *finish_text;

extern void animation_proceed(void);
extern void animation_reset(void);

extern void animation_started(Animation *animation, void *data);
extern void animation_stopped(Animation *animation, bool finished, void *data);
extern void destroy_property_animation(PropertyAnimation **prop_animation);

#endif
