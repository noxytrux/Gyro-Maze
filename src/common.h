//
//  common.h
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#ifndef GyroMaze_common_h
#define GyroMaze_common_h

#include "pebble.h"

#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define ABS( a ) ( (a) < 0 ? (-a) : (a) )

typedef struct Vec2d {
    float x;
    float y;
} Vec2d;

//structure that descibes our BALL
typedef struct Disc {
    Vec2d pos;
    Vec2d vel;
    float radius;
} Disc;

typedef enum {
    startInside = 1 << 1,
    endInside = 1 << 2,
    middleInside = 1 << 3,
    noneInside = 1 << 4
} collisionside;

extern bool circlesColliding(int x1,int y1,int radius1, int x2,int y2,int radius2);
extern collisionside intersectlinecircle(Vec2d location, double radius, Vec2d lineFrom, Vec2d lineTo);

#endif
